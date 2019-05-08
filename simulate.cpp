#include <string>
#include <sstream>
#include <iostream>
#include "simulate.h"
#include "waterPoint.h"
#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include "globals.h"
#include "KDTree/KDTree.hpp"

void Simulate::generate_initial_positions(std::vector<WaterPoint*> *water_points, float dist, int x_dim, int y_dim, int z_dim) {
  Vector3D initial_pos = Vector3D(-0.2, -0.35, -0.3);

  // CREATE WATER POINTS
  for (int z = 0; z < z_dim; z++) {
      for (int y = 0; y < y_dim; y++) {
          for (int x = 0; x < x_dim; x++) {
              Vector3D offsets = Vector3D(x * dist, y * dist, z * dist);
              WaterPoint *point = new WaterPoint(initial_pos + offsets);
              point->forces = Vector3D(0, 0, 0);
              water_points->push_back(point);
          }
      }
  }

  // CREATE BOAT POINTS
  float x, y, z;
  char c;
  std::ifstream boatfile ("smallboatmorepoints.obj");
  std::string line;
  while (std::getline(boatfile, line)) {
      if (line[0] == *"v") {
          std::istringstream iss(line);
          iss >> c >> x >> y >> z;
          WaterPoint *point = new WaterPoint(Vector3D(x, y, z));
          point->forces = Vector3D(0, 0, 0);
          point->isBoat = true;
          water_points->push_back(point);
      }
  }
}

void Simulate::simulate(std::vector<WaterPoint*> *water_points, float dt, float mass) {

    // ADD GRAVITY AS FORCE ACTING ON ALL WATER POINTS
    for (int i = 0; i < water_points->size(); i++) {
        (*water_points)[i]->forces = gravity * mass;
    }

    for (int i = 0; i < water_points->size(); i++) {
        point_t point;
        point = {(*water_points)[i]->position.x, (*water_points)[i]->position.y, (*water_points)[i]->position.z};
        auto neighbors = neighbor_tree.neighborhood_points(point, 0.55);
    }
}
