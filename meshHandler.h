#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Polyhedron_3.h>
#include <vector>
#include <fstream>
#include <waterPoint.h>

// Types
typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
typedef Kernel::Point_3 Point;
typedef Kernel::Vector_3 Vector;
typedef std::pair<Point, Vector> Pwn;
typedef CGAL::Polyhedron_3<Kernel> Polyhedron;

class MeshHandler {
  public:

  void save_dae(std::vector<WaterPoint*> *water_points, int i, char *dae_folder, char *png_folder);

  std::map<WaterPoint*, Vector> surface_points(std::vector<WaterPoint*> *water_points);

  Polyhedron water_mesh(std::map<WaterPoint*, Vector> surface_points);

  Vector find_normal(WaterPoint w, std::vector<WaterPoint*> *water_points);

};
