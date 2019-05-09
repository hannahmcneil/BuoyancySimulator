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
#include "globals.h"
#include "KDTree/KDTree.hpp"

#include <map>

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

WaterPoint* get_waterPoint_from_location_3(Vector3D v) {
  std::vector<double> vect;
  vect.push_back(v.x);
  vect.push_back(v.y);
  vect.push_back(v.z);
  return water_map[vect];
}

void MeshHandler::save_png_and_combine_frames(int time_steps, char *png_folder, char *obj_folder) {
    std::string s(png_folder);
    std::string d(obj_folder);
    for (int i = 0; i < time_steps; i++) {
        std::string png_string = "-f ./" + s + "/frame_" + std::to_string(i) + ".png ";
        std::string obj_string = "./" + d + "/frame_" + std::to_string(i) + ".obj";
        // DUMMY RENDERER
        std::string command_string = "./pathtracer -t 8 -r 480 360 " + png_string + obj_string;
    }
    return;
}

void MeshHandler::save_obj(std::vector<WaterPoint*> *water_points, int i, char *obj_folder, int num_particles) {
  std::cout << "extracting surface points" << std::endl;
  std::cout << "num water points: " << water_points->size() << std::endl;
  std::map<WaterPoint *, Vector> surface = surface_points(water_points);
  std::cout << "actual number of surface points" << std::endl;
  std::cout << surface.size() << std::endl;

  std::cout << "reconstructing mesh" << std::endl;
  Polyhedron surface_mesh = water_mesh(surface);

  std::string d(obj_folder);

  std::ofstream ofs("./" + d + "/MeshFile_Frame" + std::to_string(i) + ".obj");
  std::ofstream temp("./" + d + "/WaterMesh_Frame" + std::to_string(i) + ".obj");

  // GIVEN .OBJ FILE WITH WATER MESH AND OUR BOAT.OBJ FILE, COMBINE THE TWO; FIRST ADD BOAT
  ofs << "o Boat_Mesh" << "\n";
  for (int i = 0; i < water_points->size(); i++) {
      if ((*water_points)[i]->isBoat == true) {
          Vector3D pos = (*water_points)[i]->position;
          ofs << "v " + std::to_string(pos.x) + " " + std::to_string(pos.y) + " " + std::to_string(pos.z) << "\n";
      }
  }

  int num_vertices = 0;
  std::ifstream boatfile ("small4points.obj");
  std::string line;
  while (std::getline(boatfile, line)) {
      if ((line[0] == *"s") || (line[0] == *"f")) {
         ofs << line << "\n";
      } else if (line[0] == *"v") {
         num_vertices++;
      }
  }

  // WRITE WATER MESH TO FILE
  ofs << "o Water_Mesh" << "\n";
  CGAL::print_polyhedron_wavefront(temp, surface_mesh);

  std::cout << "combining water mesh and boat mesh into single obj file" << std::endl;

  std::ifstream file("./" + d + "/WaterMesh_Frame" + std::to_string(i) + ".obj");

  std::string str;

  std::string vertex_string;
  std::vector<Vector3D> vertices;

  std::string face_string;
  std::vector<Vector3D> faces;

  std::string normal_string;

  float x, y, z;
  int m, n, p;
  char c;
  int num_vertices_water = 0;
  
  while (std::getline(file, str)) {
    if (str[0] == *"v") {
      std::istringstream iss(str);
      iss >> c >> x >> y >> z;
      vertices.push_back(Vector3D(x, y, z));
      vertex_string += str.substr(1);
      ofs << str << "\n";
      num_vertices_water++;
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

  int total = num_vertices + num_vertices_water + 1;

  // WRITE WALLS TO FILE
  ofs << "o Back Wall" << "\n";
  ofs << "v " + std::to_string(vertex_1.x) + " " + std::to_string(vertex_1.y) + " " + std::to_string(vertex_1.z) << "\n";
  ofs << "v " + std::to_string(vertex_2.x) + " " + std::to_string(vertex_2.y) + " " + std::to_string(vertex_2.z) << "\n";
  ofs << "v " + std::to_string(vertex_3.x) + " " + std::to_string(vertex_3.y) + " " + std::to_string(vertex_3.z) << "\n";
  ofs << "v " + std::to_string(vertex_4.x) + " " + std::to_string(vertex_4.y) + " " + std::to_string(vertex_4.z) << "\n";
  ofs << "f " + std::to_string(total) + " " + std::to_string(total + 1) + " " + std::to_string(total + 2) << "\n";
  ofs << "f " + std::to_string(total + 1) + " " + std::to_string(total + 2) + " " + std::to_string(total + 3) << "\n";

  ofs << "o Left Wall" << "\n";
  ofs << "v " + std::to_string(vertex_5.x) + " " + std::to_string(vertex_5.y) + " " + std::to_string(vertex_5.z) << "\n";
  ofs << "v " + std::to_string(vertex_7.x) + " " + std::to_string(vertex_7.y) + " " + std::to_string(vertex_7.z) << "\n";
  ofs << "f " + std::to_string(total + 4) + " " + std::to_string(total) + " " + std::to_string(total + 5) << "\n";
  ofs << "f " + std::to_string(total) + " " + std::to_string(total + 5) + " " + std::to_string(total + 2) << "\n";

  ofs << "o Right Wall" << "\n";
  ofs << "v " + std::to_string(vertex_6.x) + " " + std::to_string(vertex_6.y) + " " + std::to_string(vertex_6.z) << "\n";
  ofs << "v " + std::to_string(vertex_8.x) + " " + std::to_string(vertex_8.y) + " " + std::to_string(vertex_8.z) << "\n";
  ofs << "f " + std::to_string(total + 6) + " " + std::to_string(total + 1) + " " + std::to_string(total + 7) << "\n";
  ofs << "f " + std::to_string(total + 1) + " " + std::to_string(total + 7) + " " + std::to_string(total + 3) << "\n";

  ofs << "o Floor" << "\n";
  ofs << "f " + std::to_string(total + 2) + " " + std::to_string(total + 3) + " " + std::to_string(total + 5) << "\n";
  ofs << "f " + std::to_string(total + 3) + " " + std::to_string(total + 5) + " " + std::to_string(total + 7) << "\n";

  ofs << "o Ceiling" << "\n";
  ofs << "f " + std::to_string(total) + " " + std::to_string(total + 1) + " " + std::to_string(total + 4) << "\n";
  ofs << "f " + std::to_string(total + 1) + " " + std::to_string(total + 4) + " " + std::to_string(total + 6) << "\n";

  num_particles_dim = num_particles;

  // now that we have the vertex array, we need to loop over again to find the normals

  // generate a list of pairs of vertex coordinates and normals

  // sort so they're in the same order as the vertex list

  // put the normals into a string

  //std::cout << vertex_string << std::endl;
  //std::cout << face_string << std::endl;
  //std::cout << normal_string << std::endl;

}


std::map<WaterPoint*, Vector> MeshHandler::surface_points(std::vector<WaterPoint*> *water_points) {

  //std::cout << "finding surface points" << std::endl;

  std::map<WaterPoint*, Vector> output;

  for (WaterPoint *w : *water_points) {
    if (!w->isBoat) {
      std::pair<Vector, float> pair = find_normal(*w, water_points);
      Vector normal = pair.first;
      float length = pair.second;
      if (length > particle_dist / 4.) {
        std::pair<WaterPoint*, Vector> pp = std::pair<WaterPoint*, Vector>(w, normal);
        output.insert(pp);
      }
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
  std::cout << points.size() << std::endl;
  double average_spacing = CGAL::compute_average_spacing<CGAL::Sequential_tag>
          (points, 6, CGAL::parameters::point_map(CGAL::First_of_pair_property_map<Pwn>()));

  std::cout << "surface reconstruction" << std::endl;
  /*for (Pwn point : points) {
    std::cout << point.first.x() << std::endl;
    std::cout << point.first.y() << std::endl;
    std::cout << point.first.z() << std::endl;
  }
  */

  CGAL::poisson_surface_reconstruction_delaunay
          (points.begin(), points.end(),
           CGAL::First_of_pair_property_map<Pwn>(),
           CGAL::Second_of_pair_property_map<Pwn>(),
           output_mesh, average_spacing);

  std::cout << "not surface reconstruction" << std::endl;

  return output_mesh;

}

std::pair<Vector, float> MeshHandler::find_normal(WaterPoint w, std::vector<WaterPoint*> *water_points) {


  /**std::map<double, WaterPoint*> point_distances;
  std::array<WaterPoint*, 10> neighbors;
  Vector3D average_neighbor = Vector3D(0, 0, 0);
  for (WaterPoint *neighbor : *water_points) {
      if (neighbor == &w || neighbor->isBoat == true) {
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
  **/
  Vector3D average_neighbor = Vector3D(0, 0, 0);
  point_t point;
  point = {w.position.x, w.position.y, w.position.z};
  auto neighbors_ = neighbor_tree.neighborhood_points(point, 1.5 * particle_dist);
  int num_non_boat = 0;
  for (int i = 0; i < neighbors_.size(); i++) {
    Vector3D pos = Vector3D(neighbors_[i][0], neighbors_[i][1], neighbors_[i][2]);
    WaterPoint *n = get_waterPoint_from_location_3(pos);
    if (!(n->isBoat)) {
      num_non_boat += 1;
      average_neighbor += pos;
    }
  }
  if (num_non_boat == 0) {
    return std::pair<Vector, float>(Vector(0, 0, 0), 0);
  }
  average_neighbor = average_neighbor / num_non_boat;
  Vector3D diff_vec = (average_neighbor - w.position);
  Vector3D norm = -1. * diff_vec;
  float length = norm.norm();
  norm.normalize();
  Vector normal = Vector(norm.x, norm.y, norm.z);

  return std::pair<Vector, float>(normal, length);
}

/*void MeshHandler::min_max_dim(std::vector<WaterPoint*> *water_points) {
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
*/
