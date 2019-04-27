#include <string>
#include <sstream>
#include <iostream>
#include "simulate.h"
#include "waterPoint.h"
#include <stdio.h>
#include <stdlib.h>
#include <vector>

void Simulate::generate_initial_positions(std::vector<WaterPoint*> *water_points, float particle_dist, int num_particles) {
  Vector3D initial_pos = Vector3D(-0.5, 0.25, -0.5);
  for (int z = 0; z < num_particles; z++) {
      for (int y = 0; y < num_particles; y++) {
          for (int x = 0; x < num_particles; x++) {
              Vector3D offsets = Vector3D(x * particle_dist, y * particle_dist, z * particle_dist);
              WaterPoint point = WaterPoint(initial_pos + offsets);
              water_points->push_back(&point);
          }
      }
  }
}

void Simulate::simulate(std::vector<WaterPoint*> *water_points, float dt) {
  return;
}
