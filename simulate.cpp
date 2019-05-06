#include <string>
#include <sstream>
#include <iostream>
#include "simulate.h"
#include "waterPoint.h"
#include <stdio.h>
#include <stdlib.h>
#include <vector>

void Simulate::generate_initial_positions(std::vector<WaterPoint*> *water_points, float particle_dist, int x_particles, int y_particles, int z_particles) {
  Vector3D initial_pos = Vector3D(-0.2, -0.35, -0.2);
  for (int z = 0; z < z_particles; z++) {
      for (int y = 0; y < y_particles; y++) {
          for (int x = 0; x < x_particles; x++) {
              Vector3D offsets = Vector3D(x * particle_dist, y * particle_dist, z * particle_dist);
              WaterPoint *point = new WaterPoint(initial_pos + offsets);
              point->forces = Vector3D(0, 0, 0);
              water_points->push_back(point);
          }
      }
  }
}

void Simulate::simulate(std::vector<WaterPoint*> *water_points, float dt, float mass) {

    // ADD GRAVITY AS FORCE ACTING ON ALL WATER POINTS
    for (int i = 0; i < water_points->size(); i++) {
        (*water_points)[i]->forces = gravity * mass;
    }
}
