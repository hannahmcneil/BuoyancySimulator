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
#include "KDTree/KDTree.hpp"

WaterPoint* get_waterPoint_from_location_2(Vector3D v) {
  std::vector<double> vect;
  vect.push_back(v.x);
  vect.push_back(v.y);
  vect.push_back(v.z);
  return water_map[vect];
}


void Simulate::generate_initial_positions(std::vector<WaterPoint*> *water_points, float dist, int x_dim, int y_dim, int z_dim) {
  Vector3D initial_pos = Vector3D(-0.2, -0.35, -0.3);

  // CREATE WATER POINTS
  for (int z = 0; z < z_dim; z++) {
      for (int y = 0; y < y_dim; y++) {
          for (int x = 0; x < x_dim; x++) {
              Vector3D offsets = Vector3D(x * dist, y * dist, z * dist);
              WaterPoint *point = new WaterPoint(initial_pos + offsets);
              water_points->push_back(point);
          }
      }
  }

  // CREATE BOAT POINTS
  float x, y, z;
  char c;
  std::ifstream boatfile ("build/smallboatmorepoints.obj");
  std::string line;
  while (std::getline(boatfile, line)) {
      if (line[0] == *"v") {
          std::istringstream iss(line);
          iss >> c >> x >> y >> z;
          WaterPoint *point = new WaterPoint(Vector3D(x, y, z));
          point->isBoat = true;
          water_points->push_back(point);
      }
  }
}

float W(float r) {
  return 1.56668 / pow(h, 9) * pow(h * h - r * r , 3);
}

Vector3D Grad_W(Vector3D r) {
  return 1.56668147106 / pow(h, 9) * 3 * pow(h * h - r.norm() * r.norm(), 2) * -2 * r;
}

float rho(int i, std::vector<Vector3D> neighbors) {
  float sum = 0;
  for (int j = 0; j < neighbors.size(); ++j) {
    sum += W((neighbors[i] - neighbors[j]).norm());
  }
  return sum;
}

float C(int i, std::vector<Vector3D> neighbors) {
  return rho(i, neighbors) / rho_0 - 1;
}

Vector3D Grad_C(int k, int i, std::vector<Vector3D> neighbors) {
  if (i == k) {
    Vector3D sum = 0;
    for (int j = 0; j < neighbors.size(); ++j) {
      sum += Grad_W(neighbors[i] - neighbors[j]);
    }
    return 1. / rho_0 * sum;
  }
  return 1. / rho_0 * Grad_W(neighbors[i] - neighbors[k]);
}

float lambda(int i, std::vector<Vector3D> neighbors) {
  float sum = 0;
  for (int k = 0; k < neighbors.size(); ++k) {
    sum += Grad_C(k, i, neighbors).norm();
  }
  return -C(i, neighbors) / (sum + epsilon);
}

Vector3D delta_p(int i, std::vector<Vector3D> neighbors) {
  Vector3D sum = 0;
  for (int j = 0; j < neighbors.size(); ++j) {
    // can add tensile instability term here later if we want
    sum += (lambda(i, neighbors) + lambda(j, neighbors)) * Grad_W(neighbors[i] - neighbors[j]);
  }
  return 1. / rho_0 * sum;
}

void Simulate::simulate(std::vector<WaterPoint*> *water_points, float dt) {

  for (int i = 0; i < water_points->size(); i++) {

    std::cout << (100. * i / water_points->size()) << "% complete" << std::endl;

    WaterPoint *p = (*water_points)[i];

    if (p->isBoat) {
      continue;
    }

    // finding neigbors
    std::cout << "finding neighbors" << std::endl;

    point_t point;
    point = {(*water_points)[i]->position.x, (*water_points)[i]->position.y, (*water_points)[i]->position.z};
    auto neighbors_ = neighbor_tree.neighborhood_points(point, (max_z - min_z) / 5.);

    std::vector<Vector3D> neighbor_positions;
    for (int j = 0; j < neighbors_.size(); ++j) {
      neighbor_positions.push_back(Vector3D(neighbors_[j][0], neighbors_[j][1], neighbors_[j][2]));
    }

    std::vector<WaterPoint*> neighbors;
    for (int j = 0; j < neighbors_.size(); ++j) {
      WaterPoint *w = get_waterPoint_from_location_2(neighbor_positions[j]);
      if (w == p) {
        continue;
      }
      neighbors.push_back(w);
    }

    std::vector<WaterPoint*> water_neighbors;
    std::vector<WaterPoint*> boat_neighbors;
    for (int j = 0; j < neighbors.size(); ++j) {
      if (neighbors[j]->isBoat) {
        boat_neighbors.push_back(get_waterPoint_from_location_2(neighbor_positions[j]));
      } else {
        water_neighbors.push_back(get_waterPoint_from_location_2(neighbor_positions[j]));
      }
    }

    std::vector<Vector3D> water_neighbor_positions;
    for (int j = 0; j < water_neighbors.size(); ++j) {
      water_neighbor_positions.push_back(water_neighbors[j]->position);
    }

    std::cout << "num boat neighbors: " << boat_neighbors.size() << std::endl;
    std::cout << "num water neighbors: " << water_neighbors.size() << std::endl;

    // move particle from velocity and gravity
    std::cout << "moving particle from velocity and gravity" << std::endl;

    Vector3D temp = p->position;
    p->position = 2. * p->position - p->last_position + Vector3D(0., -10., 0.) * dt * dt;
    p->last_position = temp;

    // move particle from boat collisions
    std::cout << "moving particle from boat collisions" << std::endl;

    for (int j = 0; j < boat_neighbors.size(); ++j) {
      Vector3D diff = (neighbor_positions[j] - p->position);
      float diff_norm = diff.norm();
      if (diff_norm < 0.01) {
        p->position += diff / diff.norm() * 0.01;
      }
    }

    // move particle from plane collisions
    std::cout << "moving particle from plane collisions" << std::endl;

    if (p->position.x < min_x) {
      p->position.x = min_x;
    }
    if (max_x < p->position.x) {
      p->position.x = max_x;
    }
    if (p->position.y < min_y) {
      p->position.y = min_y;
    }
    if (max_y < p->position.y) {
      p->position.y = max_y;
    }
    if (p->position.z < min_z) {
      p->position.z = min_z;
    }
    if (max_z < p->position.z) {
      p->position.z = max_z;
    }

    // move particle from water collisions
    std::cout << "moving particles from water collisions" << std::endl;

    p->position += delta_p(i, water_neighbor_positions);
  }

  return;
}
