#include "meshHandler.h"


#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/HalfedgeDS_default.h>
#include <CGAL/HalfedgeDS_decorator.h>
#include <CGAL/poisson_surface_reconstruction.h>
#include <vector>
#include <fstream>
#include <waterPoint.h> 
#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>

#include <CGAL/IO/print_wavefront.h>


// Types
typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
typedef Kernel::Point_3 Point;
typedef Kernel::Vector_3 Vector;
typedef std::pair<Point, Vector> Pwn;
typedef CGAL::Polyhedron_3<Kernel> Polyhedron;

struct Traits { typedef int Point_2; };
typedef CGAL::HalfedgeDS_default<Traits> HDS;
typedef CGAL::HalfedgeDS_decorator<HDS>  Decorator;
typedef HDS::Halfedge_iterator           Iterator;


Polyhedron output_mesh;

void MeshHandler::save_png_and_combine_frames(int time_steps, char *png_folder) {
    //for (int i = 0; i < time_steps; i++) {
      //  system("pathtracer.exe '-t 8' '-r 480 360' '-f ./png_folder/frame_i.png' ./dae_folder/frame_i.dae");
    //}
    return;
}

void MeshHandler::save_dae(std::vector<WaterPoint*> *water_points, int i, char *dae_folder) {
  std::map<WaterPoint *, Vector> surface = surface_points(water_points);
  Polyhedron surface_mesh = water_mesh(surface);

  std::cout << "saving obj file" << std::endl;

  std::ofstream ofs("MeshFile.obj");
  CGAL::print_polyhedron_wavefront(ofs, surface_mesh);

  std::cout << "reading obj file" << std::endl;

  std::ifstream file("MeshFile.obj");
  std::string str;

  std::string vertex_string;
  std::vector<Vector3D> vertices;

  std::string face_string;
  std::vector<Vector3D> faces;

  std::string normal_string;

  float x, y, z;
  int m, n, p;
  char c;

  while (std::getline(file, str)) {
    if (str[0] == *"v") {
      std::istringstream iss(str);
      iss >> c >> x >> y >> z;
      vertices.push_back(Vector3D(x, y, z));
      vertex_string += str.substr(1);
    }
    if (str[0] == *"f") {
      std::istringstream iss(str);
      iss >> c >> m >> n >> p;
      faces.push_back(Vector3D(m, n, p));
      face_string += str.substr(2);
    }
  }

  // now that we have the vertex array, we need to loop over again to find the normals

  for (int i = 0; i < faces.size(); ++i) {
    Vector3D face = faces[i];
    Vector3D v1 = vertices[face.x];
    Vector3D v2 = vertices[face.y];
    Vector3D v3 = vertices[face.z];

    Vector3D normal = cross(v2 - v1, v3 - v1);
    normal.normalize();

    normal_string += std::to_string(normal.x) + " " + std::to_string(normal.y) + " " + std::to_string(normal.z) + " ";
  }

  std::cout << vertex_string << std::endl;
  std::cout << face_string << std::endl;
  std::cout << normal_string << std::endl;

}

std::map<WaterPoint*, Vector> MeshHandler::surface_points(std::vector<WaterPoint*> *water_points) {

  std::cout << "finding surface points" << std::endl;

  std::map<WaterPoint*, Vector> output;
  for (WaterPoint *w : *water_points) {
    std::pair<Vector, float> pair = find_normal(*w, water_points);
    Vector normal = pair.first;
    float length = pair.second;
    if (length > 0.1) {
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

  std::cout << "converting points to mesh" << std::endl;

  double average_spacing = CGAL::compute_average_spacing<CGAL::Sequential_tag>
          (points, 6, CGAL::parameters::point_map(CGAL::First_of_pair_property_map<Pwn>()));

  CGAL::poisson_surface_reconstruction_delaunay
          (points.begin(), points.end(),
           CGAL::First_of_pair_property_map<Pwn>(),
           CGAL::Second_of_pair_property_map<Pwn>(),
           output_mesh, average_spacing);

  return output_mesh;

}

std::pair<Vector, float> MeshHandler::find_normal(WaterPoint w, std::vector<WaterPoint*> *water_points) {
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
  Vector3D norm = -1. * diff_vec;
  float length = norm.norm();
  norm.normalize();
  Vector normal = Vector(norm.x, norm.y, norm.z);
  return std::pair<Vector, float>(normal, length);

}
