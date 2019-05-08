#include <iostream>
#include <fstream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <KDTree/KDTree.hpp>
#ifdef _WIN32
#include "misc/getopt.h" // getopt for windows
#else
#include <getopt.h>
#include <unistd.h>
#endif

extern int NUM_PARTICLES;
extern float min_x;
extern float max_x;
extern float min_y;
extern float max_y;
extern float min_z;
extern float max_z;
extern Vector3D vertex_1;
extern Vector3D vertex_2;
extern Vector3D vertex_3;
extern Vector3D vertex_4;
extern Vector3D vertex_5;
extern Vector3D vertex_6;
extern Vector3D vertex_7;
extern Vector3D vertex_8;
extern int x_particles;
extern int y_particles;
extern int z_particles;

extern std::map<std::vector<double>, WaterPoint*> water_map;
extern KDTree neighbor_tree;
