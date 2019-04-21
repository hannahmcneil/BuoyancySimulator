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
#include "waterPoint.cpp"
#include "simulate.cpp"
#include "meshHandler.cpp"


int main() {

  float dt = 0.05;
  int num_time_steps = 100;
  int num_time_steps_per_frame = 1;

  std::vector<WaterPoint*> *water_points;

  Simulate s = Simulate();
  MeshHandler m = MeshHandler();

  s.generate_initial_positions(water_points);

  for (int i = 0; i < num_time_steps; ++i) {
    if (i % num_time_steps_per_frame == 0) {
      m.save_dae(water_points, i);
    }
    s.simulate(water_points, dt);
  }
  m.save_dae(water_points, num_time_steps);

}
