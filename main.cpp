#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
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


int main() {

  std::cout << "starting" << std::endl;

  Simulate s = Simulate();
  MeshHandler m = MeshHandler();

  int num_particles_per_dimension = 51;
  float width = 1.;
  float height = 1.;
  float length = 1.;
  float density = 997.;
  float mass = width * height * length * density / num_particles_per_dimension / num_particles_per_dimension / num_particles_per_dimension;
  float particle_dist = 1. / (num_particles_per_dimension - 1);
  float dt = 0.05;
  int num_time_steps = 100;
  int num_time_steps_per_frame = 1;

  // CREATE VECTOR OF WATER POINTS
  std::vector<WaterPoint*> water_points;

  // RESERVE SPACE TO AVOID SEGFAULTS
  water_points.reserve(num_particles_per_dimension * num_particles_per_dimension * num_particles_per_dimension);

  // SET INITIAL POSITIONS OF WATER POINTS
  s.generate_initial_positions(&water_points, particle_dist, num_particles_per_dimension);

  // RUN SIMULATION FOR NUM_TIME_STEPS
  for (int i = 0; i < num_time_steps; ++i) {
    if (i % num_time_steps_per_frame == 0) {
      m.save_dae(&water_points, i);
    }
    s.simulate(&water_points, dt, mass);
  }
  m.save_dae(&water_points, num_time_steps);

  std::cout << "done" << std::endl;

}
