#include <waterPoint.h>

class Simulate {

  public:
  void generate_initial_positions(std::vector<WaterPoint*> *water_points, float particle_dist, int x_particles, int y_particles, int z_particles);

  void simulate(std::vector<WaterPoint*> *water_points, float dt, int time_step);

  CGL::Vector3D gravity = CGL::Vector3D(0, -9.8, 0);

};
