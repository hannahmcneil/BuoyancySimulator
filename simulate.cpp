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
