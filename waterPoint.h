#include "CGL/CGL.h"
#include "CGL/misc.h"
#include "CGL/vector3D.h"

using namespace CGL;

#ifndef WATERPOINT
#define WATERPOINT

class WaterPoint {
  public:
  WaterPoint(Vector3D position)
          : position(position),
            last_position(position) {}

  Vector3D velocity(double delta_t) {
    return (position - last_position) / delta_t;
  }

  // dynamic values
  Vector3D position;
  Vector3D last_position;

  bool isBoat = false;

};


#endif
