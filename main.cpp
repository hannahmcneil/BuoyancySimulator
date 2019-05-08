#include <iostream>
#include <fstream>
#include <stdio.h>
#include <vector>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <map>

#ifdef _WIN32
#include "misc/getopt.h" // getopt for windows
#else
#include <getopt.h>
#include <unistd.h>
#endif
#include <unordered_set>
#include <stdlib.h> // atoi for getopt inputs

#include "CGL/CGL.h"
#include "simulate.cpp"
#include "meshHandler.cpp"
#include "globals.h"
#include "waterPoint.h"
#include "waterPoint.cpp"


#include "KDTree/KDTree.hpp"
#include "KDTree/KDTree.cpp"


int NUM_PARTICLES = 0;

// ROOM DIMENSIONS
float min_x = -0.3;
float max_x = 0.3;
float min_y = -0.6;
float max_y = 0.3;
float min_z = -0.4;
float max_z = 0.5;

// ROOM VERTICES
Vector3D vertex_1 = Vector3D(min_x, max_y, max_z);
Vector3D vertex_2 = Vector3D(min_x, max_y, min_z);
Vector3D vertex_3 = Vector3D(min_x, min_y, max_z);
Vector3D vertex_4 = Vector3D(min_x, min_y, min_z);
Vector3D vertex_5 = Vector3D(max_x, max_y, max_z);
Vector3D vertex_6 = Vector3D(max_x, max_y, min_z);
Vector3D vertex_7 = Vector3D(max_x, min_y, max_z);
Vector3D vertex_8 = Vector3D(max_x, min_y, min_z);

// Euler angles for boat
float phi = 0;
float theta = 0;
float phi_prime = 0;

// Previous euler angles for boat
float phi_prev = 0;
float theta_prev = 0;
float phi_prime_prev = 0;

// Angular velocity of boat
float wx = 0;
float wy = 0;
float wz = 0;


std::map<std::vector<double>, WaterPoint*> water_map = std::map<std::vector<double>, WaterPoint*>();

KDTree neighbor_tree = KDTree();

WaterPoint* get_waterPoint_from_location(Vector3D v) {
  std::vector<double> vect;
  vect.push_back(v.x);
  vect.push_back(v.y);
  vect.push_back(v.z);
  return water_map[vect];
}

void create_map(std::vector<WaterPoint*> water_points) {
  water_map.clear();
  for (int i = 0; i < water_points.size(); i++) {
    WaterPoint *p = water_points[i];
    std::vector<double> vect;
    vect.push_back(p->position.x);
    vect.push_back(p->position.y);
    vect.push_back(p->position.z);
    water_map[vect] = p;
  }
}

void populate_tree(std::vector<WaterPoint*> water_points) {
    pointVec points;
    for (int i = 0; i < water_points.size(); i++) {
        WaterPoint *p = water_points[i];
        point_t point = {p->position.x, p->position.y, p->position.z};
        points.push_back(point);
    }
    neighbor_tree = KDTree(points);
}


int main(int argc, char **argv) {

  /* std::cout << "starting KDTree test" << std::endl;

  pointVec points;
  point_t pt;

  pt = {0.0, 0.0};
  points.push_back(pt);
  pt = {1.0, 0.0};
  points.push_back(pt);
  pt = {0.0, 1.0};
  points.push_back(pt);
  pt = {1.0, 1.0};
  points.push_back(pt);
  pt = {0.5, 0.5};
  points.push_back(pt);

  KDTree tree = KDTree(points);

  std::cout << "nearest test\n";
  pt = {0.8, 0.2};
  auto res = tree.nearest_point(pt);
  for (double b : res) {
    std::cout << b << " ";
  }
  std::cout << '\n';

  std::cout << "nearest in range test\n";

  auto res2 = tree.neighborhood_points(pt, .55);

  for (point_t a : res2) {
    for (double b : a) {
      std::cout << b << " ";
    }
    std::cout << '\n';
  }

  std::cout << "ending KDTree test" << std::endl;
  */


  if (argc != 3) {
      printf("please provide names for 2 destination folders\n");
      exit(EXIT_FAILURE);
  }

  // COUNT NUMBER OF BOAT POINTS
  int num_vertices = 0;
  std::ifstream boatfile ("smallboatmorepoints.obj");
  std::string line;
  while (std::getline(boatfile, line)) {
      if (line[0] == *"v") {
          num_vertices++;
      }
  }


  int check1;
  int check2;
  check1 = mkdir(argv[1], S_IRWXU);
  check2 = mkdir(argv[2], S_IRWXU);

  std::cout << "starting main loop" << std::endl;

  Simulate s = Simulate();
  MeshHandler m = MeshHandler();

  // BELOW, USED TO CALCULATE TOTAL NUMBER OF PARTICLES
  float width = .75;
  float height = 0.3;
  float length = 0.4;
  float particle_dist = (1. / 10.);

  // CALCULATE NUMBER OF PARTICLES
  int z_particles = ((int) (width / particle_dist)) + 1;
  int y_particles = ((int) (height / particle_dist)) + 1;
  int x_particles = ((int) (length / particle_dist)) + 1;
  NUM_PARTICLES = x_particles * y_particles * z_particles;
  
  // WATERPOINT PARAMETERS
  float density = 997.;
  float mass = width * height * length * density / x_particles / y_particles / z_particles;
  
  // FRAMERATE PARAMETERS
  float dt = 0.05;
  int num_time_steps = 5;
  int num_time_steps_per_frame = 1;

  // CREATE VECTOR OF WATER POINTS
  std::vector<WaterPoint*> water_points;

  // RESERVE SPACE TO AVOID SEGFAULTS
  water_points.reserve(x_particles * y_particles * z_particles + num_vertices);

  // SET INITIAL POSITIONS OF WATER POINTS
  std::cout << "Generate initial frame:" << std::endl;
  std::cout << "setting initial positions" << std::endl;
  s.generate_initial_positions(&water_points, particle_dist, x_particles, y_particles, z_particles);
  populate_tree(water_points);
  create_map(water_points);

  // RUN SIMULATION FOR NUM_TIME_STEPS
  for (int i = 0; i < num_time_steps; ++i) {
    if (i % num_time_steps_per_frame == 0) {
      std::cout << "saving .obj file:" << std::endl;
      m.save_obj(&water_points, i, argv[1], NUM_PARTICLES);
      std::cout << " " << std::endl;
      std::cout << "Generate next frame:" << std::endl;
    }
    std::cout << "simulating movement" << std::endl;
    s.simulate(&water_points, dt, mass);
    populate_tree(water_points);
    create_map(water_points);
  }
  std::cout << "saving .obj file:" << std::endl;
  m.save_obj(&water_points, num_time_steps, argv[1], NUM_PARTICLES);

  std::cout << "render .obj files" << std::endl;
  m.save_png_and_combine_frames(num_time_steps, argv[2], argv[1]);

  std::cout << "finished main loop" << std::endl;

}
