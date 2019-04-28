#include "meshHandler.h"


#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/poisson_surface_reconstruction.h>
#include <vector>
#include <fstream>
#include <waterPoint.h> 
#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>

// Types
typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
typedef Kernel::Point_3 Point;
typedef Kernel::Vector_3 Vector;
typedef std::pair<Point, Vector> Pwn;
typedef CGAL::Polyhedron_3<Kernel> Polyhedron;

Polyhedron output_mesh;



void MeshHandler::save_dae(std::vector<WaterPoint*> *water_points, int i, char *dae_folder, char *png_folder) {
    std::map<WaterPoint*, Vector> surface = surface_points(water_points);
    Polyhedron surface_mesh = water_mesh(surface);
}

std::map<WaterPoint*, Vector> MeshHandler::surface_points(std::vector<WaterPoint*> *water_points) {
  std::map<WaterPoint*, Vector> output;
  for (WaterPoint *w : *water_points) {
    Vector normal = find_normal(*w, water_points);
    if (normal.x() * normal.x() + normal.y() * normal.y() + normal.z() * normal.z() > 0.1) {
      std::pair<WaterPoint*, Vector> pp = std::pair<WaterPoint*, Vector>(w, normal);
      output.insert(pp);
    }
  }
  return output;
}

Polyhedron MeshHandler::water_mesh(std::map<WaterPoint*, Vector> surface_points) {
  Polyhedron output_mesh;

  std::vector<Pwn> points;
 
  for(std::map<WaterPoint*, Vector>::iterator iter = surface_points.begin(); iter != surface_points.end(); ++iter) {
    WaterPoint* w =  iter->first;
    Point p = Point(w->position.x, w->position.y, w->position.z);
    Vector n = iter->second;
    points.push_back(std::pair<Point, Vector>(p, n));
  }

  double average_spacing = CGAL::compute_average_spacing<CGAL::Sequential_tag>
          (points, 6, CGAL::parameters::point_map(CGAL::First_of_pair_property_map<Pwn>()));

  std::cout << "computed average spacing" << std::endl;

  CGAL::poisson_surface_reconstruction_delaunay
          (points.begin(), points.end(),
           CGAL::First_of_pair_property_map<Pwn>(),
           CGAL::Second_of_pair_property_map<Pwn>(),
           output_mesh, average_spacing);

  std::cout << "reconstruction complete" << std::endl;

  return output_mesh;

}

Vector MeshHandler::find_normal(WaterPoint w, std::vector<WaterPoint*> *water_points) {
  std::map<double, WaterPoint*> point_distances;
  std::array<WaterPoint*, 10> neighbors;
  Vector3D average_neighbor = Vector3D(0, 0, 0);
  for (WaterPoint *neighbor : *water_points) {
      if (neighbor == &w) {
          continue;
      }
      double dist = (w.position - neighbor->position).norm();
      std::pair<double, WaterPoint*> point_dist_pair = std::pair<double, WaterPoint*>(dist, neighbor);
      point_distances.insert(point_dist_pair);
  }
  std::map<double, WaterPoint*>::iterator iter = point_distances.begin();
  for (int i = 0; i < neighbors.size(); i++) {
      neighbors[i] = iter->second;
      iter++;
  }
  for (int i = 0; i < neighbors.size(); i++) {
      average_neighbor += neighbors[i]->position;
  }
  average_neighbor = average_neighbor / 10.;
  Vector3D diff_vec = (average_neighbor - w.position);
  diff_vec.normalize();
  Vector3D norm = -1. * diff_vec;
  Vector normal = Vector(norm.x, norm.y, norm.z);
  return normal;

}
