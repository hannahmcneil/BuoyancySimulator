#include <waterPoint.h>

class Simulate {

  public:
  void generate_initial_positions(std::vector<WaterPoint*> *water_points, float particle_dist, int num_particles);

  void simulate(std::vector<WaterPoint*> *water_points, float dt, float mass);

  CGL::Vector3D gravity = CGL::Vector3D(0, -9.8, 0);

};
