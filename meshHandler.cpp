#include "meshHandler.h"


#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/HalfedgeDS_default.h>
#include <CGAL/HalfedgeDS_decorator.h>
#include <CGAL/poisson_surface_reconstruction.h>
#include <vector>
#include <fstream>
#include <waterPoint.h> 
#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>

#include <CGAL/IO/print_wavefront.h>


// Types
typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
typedef Kernel::Point_3 Point;
typedef Kernel::Vector_3 Vector;
typedef std::pair<Point, Vector> Pwn;
typedef CGAL::Polyhedron_3<Kernel> Polyhedron;

struct Traits { typedef int Point_2; };
typedef CGAL::HalfedgeDS_default<Traits> HDS;
typedef CGAL::HalfedgeDS_decorator<HDS>  Decorator;
typedef HDS::Halfedge_iterator           Iterator;


Polyhedron output_mesh;
boost::unordered_map<float, std::vector<WaterPoint *> *> waterpoint_map;
Vector3D water_min;
Vector3D water_max;
int num_particles_dim;

void MeshHandler::save_png_and_combine_frames(int time_steps, char *png_folder, char *dae_folder) {
    std::string s(png_folder);
    std::string d(dae_folder);
    for (int i = 0; i < time_steps; i++) {
        std::string png_string = "-f ./" + s + "/frame_" + std::to_string(i) + ".png ";
        std::string dae_string = "./" + d + "/frame_" + std::to_string(i) + ".dae";
        std::string command_string = "./pathtracer -t 8 -r 480 360 " + png_string + dae_string;
        //system("./pathtracer -t 8 -r 480 360 -f boat_Smallish.png boatSmallish.dae");
        //std::cout << command_string << std::endl;
    }
    return;
}

void MeshHandler::save_dae(std::vector<WaterPoint*> *water_points, int i, char *dae_folder, int num_particles_per_dimension) {
  std::map<WaterPoint *, Vector> surface = surface_points(water_points);
  Polyhedron surface_mesh = water_mesh(surface);

  std::cout << "saving obj file" << std::endl;

  std::ofstream ofs("MeshFile.obj");
  std::ofstream temp("WaterMesh.obj");

  // GIVEN .OBJ FILE WITH WATER MESH AND OUR BOAT.OBJ FILE, COMBINE THE TWO; FIRST ADD BOAT
  int num_vertices = 0;
  std::ifstream boatfile ("boat.obj");
  std::string line;
  while (std::getline(boatfile, line)) {
      if ((line[0] == *"o") || (line[0] == *"v") || (line[0] == *"s") || (line[0] == *"f")) {
         std::cout << line << std::endl;
         ofs << line << "\n";
         if (line[0] == *"v") {
             num_vertices++;
         }
      }
  }

  // WRITE WATER MESH TO FILE
  ofs << "o Water_Mesh" << "\n";
  CGAL::print_polyhedron_wavefront(temp, surface_mesh);

  std::cout << "reading obj file" << std::endl;

  std::ifstream file("WaterMesh.obj");

  std::string str;

  std::string vertex_string;
  std::vector<Vector3D> vertices;

  std::string face_string;
  std::vector<Vector3D> faces;

  std::string normal_string;

  float x, y, z;
  int m, n, p;
  char c;
  
  while (std::getline(file, str)) {
    if (str[0] == *"v") {
      std::istringstream iss(str);
      iss >> c >> x >> y >> z;
      vertices.push_back(Vector3D(x, y, z));
      vertex_string += str.substr(1);
      ofs << str << "\n";
    }
    else if (str[0] == *"f") {
      std::string updated_string;
      std::istringstream iss(str);
      iss >> c >> m >> n >> p;
      m += num_vertices;
      n += num_vertices;
      p += num_vertices;
      updated_string = "f " + std::to_string(m) + " " + std::to_string(n) + " " + std::to_string(p);
      faces.push_back(Vector3D(m, n, p));
      face_string += updated_string.substr(2);
      ofs << updated_string << "\n";
    }
  }

  num_particles_dim = num_particles_per_dimension;

  // now that we have the vertex array, we need to loop over again to find the normals

  // generate a list of pairs of vertex coordinates and normals

  // sort so they're in the same order as the vertex list

  // put the normals into a string

  std::cout << vertex_string << std::endl;
  std::cout << face_string << std::endl;
  std::cout << normal_string << std::endl;

}


std::map<WaterPoint*, Vector> MeshHandler::surface_points(std::vector<WaterPoint*> *water_points) {

  std::cout << "finding surface points" << std::endl;

  std::map<WaterPoint*, Vector> output;
  for (WaterPoint *w : *water_points) {
    std::pair<Vector, float> pair = find_normal(*w, water_points);
    Vector normal = pair.first;
    float length = pair.second;
    if (length > 0.1) {
      std::pair<WaterPoint*, Vector> pp = std::pair<WaterPoint*, Vector>(w, normal);
      output.insert(pp);
    }
  }
  return output;
}

Polyhedron MeshHandler::water_mesh(std::map<WaterPoint*, Vector> surface_points) {
  Polyhedron output_mesh;

  std::vector<Pwn> points;
 
  for(std::map<WaterPoint*, Vector>::iterator iter = surface_points.begin(); iter != surface_points.end(); ++iter) {
    WaterPoint* w =  iter->first;
    Point p = Point(w->position.x, w->position.y, w->position.z);
    Vector n = iter->second;
    points.push_back(std::pair<Point, Vector>(p, n));
  }

  std::cout << "converting points to mesh" << std::endl;

  double average_spacing = CGAL::compute_average_spacing<CGAL::Sequential_tag>
          (points, 6, CGAL::parameters::point_map(CGAL::First_of_pair_property_map<Pwn>()));

  CGAL::poisson_surface_reconstruction_delaunay
          (points.begin(), points.end(),
           CGAL::First_of_pair_property_map<Pwn>(),
           CGAL::Second_of_pair_property_map<Pwn>(),
           output_mesh, average_spacing);

  return output_mesh;

}

std::pair<Vector, float> MeshHandler::find_normal(WaterPoint w, std::vector<WaterPoint*> *water_points) {
  std::map<double, WaterPoint*> point_distances;
  std::array<WaterPoint*, 10> neighbors;
  Vector3D average_neighbor = Vector3D(0, 0, 0);
  for (WaterPoint *neighbor : *water_points) {
      if (neighbor == &w) {
          continue;
      }
      double dist = (w.position - neighbor->position).norm();
      std::pair<double, WaterPoint*> point_dist_pair = std::pair<double, WaterPoint*>(dist, neighbor);
      point_distances.insert(point_dist_pair);
  }
  std::map<double, WaterPoint*>::iterator iter = point_distances.begin();
  for (int i = 0; i < neighbors.size(); i++) {
      neighbors[i] = iter->second;
      iter++;
  }
  for (int i = 0; i < neighbors.size(); i++) {
      average_neighbor += neighbors[i]->position;
  }
  average_neighbor = average_neighbor / 10.;
  Vector3D diff_vec = (average_neighbor - w.position);
  Vector3D norm = -1. * diff_vec;
  float length = norm.norm();
  norm.normalize();
  Vector normal = Vector(norm.x, norm.y, norm.z);
  return std::pair<Vector, float>(normal, length);

}
void MeshHandler::min_max_dim(std::vector<WaterPoint*> *water_points) {
  double inf = std::numeric_limits<double>::infinity();
  water_min = Vector3D(inf, inf, inf);
  water_max = Vector3D(-inf, -inf, -inf);
  for (WaterPoint* waterpoint : *water_points) {
    Vector3D water_pos = waterpoint->position;
    if (water_pos.x > water_max.x) {
      water_max.x = water_pos.x;
    }
    if (water_pos.y > water_max.y) {
      water_max.y = water_pos.y;
    }
    if (water_pos.z > water_max.z) {
      water_max.z = water_pos.z;
    }
    if (water_pos.x < water_min.x) {
      water_min.x = water_pos.x;
    }
    if (water_pos.y < water_min.y) {
      water_min.y = water_pos.y;
    }
    if (water_pos.z < water_min.z) {
      water_min.z = water_pos.z;
    }
  }
}

float MeshHandler::find_map_index(Vector3D point) {
  float xdist = water_max.x - water_min.x;
  float ydist = water_max.y - water_min.y;
  float zdist = water_max.z - water_min.z;

  // placeholder
  return 1.;

}

void MeshHandler::build_map(std::vector<WaterPoint*> *water_points) {
  for (const auto entry: waterpoint_map) {
    delete(entry.second);
  }
  waterpoint_map.clear();
  //clear the map and set up the stuff for the indexing
  min_max_dim(water_points);

  for (WaterPoint* waterpoint : *water_points) {
    float index = find_map_index(waterpoint->position);
    if (!waterpoint_map[index]) {
      waterpoint_map[index] = new std::vector<WaterPoint*>();

    }
    waterpoint_map[index]->__emplace_back(waterpoint);
  }
}
