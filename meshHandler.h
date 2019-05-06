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

  void save_png_and_combine_frames(int time_steps, char *png_folder, char *obj_folder);

  void save_obj(std::vector<WaterPoint*> *water_points, int i, char *obj_folder, int num_particles);

  std::map<WaterPoint*, Vector> surface_points(std::vector<WaterPoint*> *water_points);

  Polyhedron water_mesh(std::map<WaterPoint*, Vector> surface_points);

  std::pair<Vector, float> find_normal(WaterPoint w, std::vector<WaterPoint*> *water_points);

  float find_map_index(Vector3D point);

  void build_map(std::vector<WaterPoint*> *water_points);

  void min_max_dim(std::vector<WaterPoint*> *water_points);
};
