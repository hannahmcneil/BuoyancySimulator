#include "meshHandler.h"


#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/poisson_surface_reconstruction.h>
#include <vector>
#include <fstream>

// Types
typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
typedef Kernel::Point_3 Point;
typedef Kernel::Vector_3 Vector;
typedef std::pair<Point, Vector> Pwn;
typedef CGAL::Polyhedron_3<Kernel> Polyhedron;

Polyhedron output_mesh;



void MeshHandler::save_dae(std::vector<WaterPoint*> *water_points, int i) {
  return;
}

std::map<WaterPoint*, Vector>* MeshHandler::surface_points(std::vector<WaterPoint*> *water_points) {
  std::map<WaterPoint*, Vector>* output;
  for (WaterPoint *w : *water_points) {
    Vector normal = find_normal(*w);
    if (normal.x() * normal.x() + normal.y() * normal.y() + normal.z() * normal.z() > 0.1) {
      output->emplace(w, normal);
    }
  }
  return output;
}

Polyhedron water_mesh(std::map<WaterPoint*, Vector>* surface_points) {
  Polyhedron output_mesh;

  std::vector<Pwn> points;

  for(std::map<WaterPoint*, Vector>::iterator iter = surface_points->begin(); iter != surface_points->end(); ++iter) {
    WaterPoint* w =  iter->first;
    Point p = Point(w->position.x, w->position.y, w->position.z);
    Vector n = iter->second;
    points.emplace_back(std::pair<Point, Vector>(p, n));
  }

  double average_spacing = CGAL::compute_average_spacing<CGAL::Sequential_tag>
          (points, 6, CGAL::parameters::point_map(CGAL::First_of_pair_property_map<Pwn>()));

  CGAL::poisson_surface_reconstruction_delaunay
          (points.begin(), points.end(),
           CGAL::First_of_pair_property_map<Pwn>(),
           CGAL::Second_of_pair_property_map<Pwn>(),
           output_mesh, average_spacing);

  return output_mesh;

}

Vector MeshHandler::find_normal(WaterPoint w) {
  return Vector(0, 0, 0);
}
