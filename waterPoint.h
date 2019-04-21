#include "CGL/CGL.h"
#include "CGL/misc.h"
#include "CGL/vector3D.h"

using namespace CGL;

class WaterPoint {
  WaterPoint(Vector3D position)
  : position(position),
  last_position(position) {}

  Vector3D velocity(double delta_t) {
    return (position - last_position) / delta_t;
  }

  public:
  // dynamic values
  Vector3D position;
  Vector3D last_position;
  Vector3D forces;

};
