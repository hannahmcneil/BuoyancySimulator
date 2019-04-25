#include <waterPoint.h>

class Simulate {

  public:
  void generate_initial_positions(std::vector<WaterPoint*> *water_points);

  void simulate(std::vector<WaterPoint*> *water_points, float dt);

};
