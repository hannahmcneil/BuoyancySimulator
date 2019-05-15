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

Vector3D rotate_x(Vector3D v, float theta) {
  Vector3D ret;
  ret.x = v.x;
  ret.y = cos(theta) * v.y - sin(theta) * v.z;
  ret.z = sin(theta) * v.y + cos(theta) * v.z;
  return ret;
}

Vector3D rotate_y(Vector3D v, float theta) {
  Vector3D ret;
  ret.x = cos(theta) * v.x + sin(theta) * v.z;
  ret.y = v.y;
  ret.z = -sin(theta) * v.x + cos(theta) * v.z;
  return ret;
}

WaterPoint* get_waterPoint_from_location_2(Vector3D v) {
  std::vector<double> vect;
  vect.push_back(v.x);
  vect.push_back(v.y);
  vect.push_back(v.z);
  return water_map[vect];
}

void create_map_2(std::vector<WaterPoint*> water_points) {
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

void populate_tree_2(std::vector<WaterPoint*> water_points) {
  pointVec points;
  for (int i = 0; i < water_points.size(); i++) {
    WaterPoint *p = water_points[i];
    point_t point = {p->position.x, p->position.y, p->position.z};
    points.push_back(point);
  }
  neighbor_tree = KDTree(points);
}


void Simulate::generate_initial_positions(std::vector<WaterPoint*> *water_points, float dist, int x_dim, int y_dim, int z_dim) {
  Vector3D initial_pos = Vector3D(-0.25, -0.55, -0.35);

  // CREATE WATER POINTS
  float off = (particle_dist/100);
  for (int z = 0; z < z_dim; z++) {
    for (int y = 0; y < y_dim; y++) {
      for (int x = 0; x < x_dim; x++) {
        double rnx = ((double)rand()/ (RAND_MAX));
        double rny = ((double)rand()/ (RAND_MAX));
        double rnz = ((double)rand()/ (RAND_MAX));
        double xoff = -off*(1. - rnx) + rnx*off;
        double yoff = -off*(1. - rny) + rny*off;
        double zoff = -off*(1. - rnz) + rnz*off;
        Vector3D offsets = Vector3D(x * dist + xoff, y * dist + yoff, z * dist + zoff);
        WaterPoint *point = new WaterPoint(initial_pos + offsets);
        point->last_position = initial_pos + offsets;
        point->next_position = initial_pos + offsets;
        point->initial_position = initial_pos + offsets;
        water_points->push_back(point);
      }
    }
  }

  // CREATE BOAT POINTS
  float x, y, z;
  char c;
  Vector3D sum = Vector3D(0,0,0);
  float count = 0.0;
  std::ifstream boatfile ("build/small4points.obj");
  std::string line;
  while (std::getline(boatfile, line)) {
    if (line[0] == *"v") {
      std::istringstream iss(line);
      iss >> c >> x >> y >> z;
      WaterPoint *point = new WaterPoint(Vector3D(x, y, z));
      point->last_position = Vector3D(x, y, z);
      point->next_position = Vector3D(x, y, z);
      point->initial_position = Vector3D(x, y, z);
      point->isBoat = true;
      water_points->push_back(point);
      sum += point->position;
      count += 1.;
    }
  }
  com = sum/count;
  com_prev = com;
  com_next = com;
  com_initial = com;
}

float W_new(float r) {
  float val = 0.179587 / (h * h * h) * exp(-r * r / (h * h));
  //std::cout << "r:" << r << std::endl;
  //std::cout << "W:" << val << std::endl;
  return val;
}

float W(float r) {
  if (abs(r) > h) {
    return 0;
  }
  float val = 1.56668 / pow(h, 9) * pow(h * h - r * r, 3);
  //std::cout << "r:" << r << std::endl;
  //std::cout << "W:" << val << std::endl;
  return val;
}

Vector3D Grad_W_new(Vector3D r) {
  //std::cout << "vector r:" << r.x << r.y << r.z << std::endl;
  Vector3D val = 0.179587 / (h * h * h) * -2 * r / (h * h) * exp(-r.norm() * r.norm() / (h * h));
  //std::cout << "grad_w:" << val.x << val.y << val.z << std::endl;
  return val;
}

Vector3D Grad_W(Vector3D r) {
  if (r.norm() > h) {
    return Vector3D(0, 0, 0);
  }
  //std::cout << "vector r:" << r.x << r.y << r.z << std::endl;
  Vector3D val = 1.56668 / pow(h, 9) * 3 * pow(h * h - r.norm() * r.norm(), 2) * -2 * r;
  //std::cout << "grad_w:" << val.x << val.y << val.z << std::endl;
  return val;
}

float rho(int i, std::vector<Vector3D> neighbors) {
  float sum = 0;
  for (int j = 0; j < neighbors.size(); ++j) {
    //std::cout << "point i pos:" << water_points[i]->position.x << water_points[i]->position.y << water_points[i]->position.z << std::endl;
    //std::cout << "neighbor j pos:" << neighbors[j].x << neighbors[j].y << neighbors[j].z << std::endl;
    sum += W((water_points[i]->position - neighbors[j]).norm());
  }
  //std::cout << "rho:" << sum << std::endl;
  return sum;
}

float C(int i, std::vector<Vector3D> neighbors) {
  float val = (rho(i, neighbors) / rho_0) - 1.;
  //std::cout << "C:" << val << std::endl;
  return val;
}

Vector3D Grad_C(int k, int i, std::vector<Vector3D> neighbors) {
  if (water_points[i]->position == neighbors[k]) {
    Vector3D sum = Vector3D(0, 0, 0);
    for (int j = 0; j < neighbors.size(); ++j) {
      sum += Grad_W(water_points[i]->position - neighbors[j]);
    }
    return 1. / rho_0 * sum;
  }
  return 1. / rho_0 * Grad_W(water_points[i]->position - neighbors[k]);
}

float lambda(int i, std::vector<Vector3D> neighbors) {
  float sum = 0;
  for (int k = 0; k < neighbors.size(); ++k) {
      float grad_c = Grad_C(k, i, neighbors).norm();
      //std::cout << "grad_c:" << grad_c << std::endl;
      sum += grad_c * grad_c;
  }
  float neg_c = -C(i, neighbors) / (sum + epsilon);
  //std::cout << "lambda:" << neg_c << std::endl;
  return neg_c;
}
float scorr(int i, int j, float wterm, std::vector<Vector3D> neighbors) {
  float k = .1;
  return -k*pow(W((water_points[i]->position - neighbors[j]).norm())*wterm,4);
}


Vector3D delta_p(int i, std::vector<Vector3D> neighbors) {
  Vector3D sum = Vector3D(0, 0, 0);
  //Vector3D scorr_sum = Vector3D();
  float wterm = 1. /W(.2*h);
  for (int j = 0; j < neighbors.size(); ++j) {
    // can add tensile instability term here later if we want
    float lamb = lambda(i, neighbors) + lambda(j, neighbors);
    //std::cout << "lambda:" << lamb << std::endl;
    Vector3D grad_w = Grad_W(water_points[i]->position - neighbors[j]);
    //std::cout << "grad_w:" << grad_w.x << grad_w.y << grad_w.z << std::endl;
    //+ scorr(i,j, wterm, neighbors)
    sum += (lamb ) * grad_w;
  }
  //sum += scorr_sum*wterm;
  return (1. / rho_0) * (sum);
}

void Simulate::simulate(std::vector<WaterPoint*> *water_points, float dt, int time_step) {

  for (int i = 0; i < water_points->size(); ++i) {
    (*water_points)[i]->next_position = (*water_points)[i]->position;
  }
  /*
  for (int i = 0; i < water_points->size(); i++) {
    WaterPoint *p = (*water_points)[i];
    if (p->isBoat) {
      continue;
    }
    if (p->position.x < min_x) {
      p->next_position.x = min_x + .001;
      p->last_position.x = min_x;
    }
    if (max_x < p->position.x) {
      p->next_position.x = max_x - .001;
      p->last_position.x = max_x;
    }
    if (p->position.y < min_y) {
      p->next_position.y = min_y + .001;
      p->last_position.y = min_y;
    }
    if (max_y < p->position.y) {
      p->next_position.y = max_y - .001;
      p->last_position.y = max_y;
    }
    if (p->position.z < min_z) {
      p->next_position.z = min_z + .001;
      p->last_position.z = min_z;
    }
    if (max_z < p->position.z) {
      p->next_position.z = max_z - .001;
      p->last_position.z = max_z;
    }
  }
  for (int i = 0; i < water_points->size(); ++i) {
    (*water_points)[i]->position = (*water_points)[i]->next_position;
  } */

  // move particle from velocity and gravity
  for (int i = 0; i < water_points->size(); i++) {
    WaterPoint *p = (*water_points)[i];
    if (p->isBoat) {
      continue;
    }
    p->next_position = 2. * p->position - p->last_position + Vector3D(0., -10., 0.) * dt * dt;
  }
  for (int i = 0; i < water_points->size(); ++i) {
    (*water_points)[i]->last_position = (*water_points)[i]->position;
    (*water_points)[i]->position = (*water_points)[i]->next_position;
  }
  if (time_step > 3) {
    // move boat from velocity and gravity
    com_next = 2. * com - com_prev + Vector3D(0., -10., 0.) * dt * dt;

    com_prev = com;
    com = com_next;

    float phi_dot = (phi - phi_prev) / dt;
    float theta_dot = (theta - theta_prev) / dt;
    float phi_prime_dot = (phi_prime - phi_prime_prev) / dt;


    std::cout << "dt: " << dt << std::endl;
    std::cout << "prev: " << phi_prev << ' ' << theta_prev << ' ' << phi_prime_prev << std::endl;
    std::cout << "cur: " << phi << ' ' << theta << ' ' << phi_prime << std::endl;
    std::cout << "dots: " << phi_dot << ' ' << theta_dot << ' ' << phi_prime_dot << std::endl;
    std::cout << "theta: " << theta << std::endl;
    std::cout << "torque: " << torque.x << ' ' << torque.y << ' ' << torque.z << std::endl;

    Vector3D w = Vector3D();
    w.x = cos(theta) * phi_dot + phi_prime_dot;
    w.y = sin(theta) * sin(phi_prime) * phi_dot + cos(phi_prime) * theta_dot;
    w.z = cos(phi_prime) * sin(theta) * phi_dot - sin(phi_prime) * theta_dot;

    if (time_step > 22) {
      std::cout << "w value: " << w.x << ' ' << w.y << ' ' << w.z << std::endl;
    }

    w += torque;

    if (abs(theta) < 0.0001) {
      theta = 0.0001;
    }

    if (time_step > 22) {
      std::cout << "w value: " << w.x << ' ' << w.y << ' ' << w.z << std::endl;
    }

    phi_dot = w.y * sin(phi_prime) / sin(theta) + w.z * cos(phi_prime) / sin(theta);
    theta_dot = w.y * cos(phi_prime) - w.z * sin(phi_prime);
    phi_prime_dot = w.x - w.y * sin(phi_prime) / tan(theta) - w.z * cos(phi_prime) / tan(theta);

    // damping
    phi += phi_dot * dt * 0.1;
    theta += theta_dot * dt * 0.1;
    phi_prime += phi_prime_dot * dt * 0.1;
  }

  for (int i = 0; i < water_points->size(); i++) {
    WaterPoint *p = (*water_points)[i];
    if (p->isBoat) {
      p->next_position = rotate_x(p->initial_position, phi);
      p->next_position = rotate_y(p->next_position, theta);
      p->next_position = rotate_x(p->next_position, phi_prime);

      p->next_position += com - com_initial;
    }
  }

  std::cout << "new step" << std::endl;
  std::cout << phi << " " << theta << " " << phi_prime << " " << std::endl;

  Vector3D yhat = Vector3D(0, 1, 0);
  yhat = rotate_x(yhat, phi);
  yhat = rotate_y(yhat, theta);
  yhat = rotate_x(yhat, phi_prime);

  std::cout << yhat.x << " " << yhat.y << " " << yhat.z << std::endl;

  Vector3D vectyBoi = Vector3D(0.0000001, 1.0000001, 0.0000001);
  vectyBoi = vectyBoi / (vectyBoi.norm() + 0.0000001);
  //vectyBoi /= 1.01;
  float theta = acos(dot(yhat, vectyBoi));
  float theta_max = 5. * PI / 180.;
  Vector3D dir = cross(yhat, vectyBoi);
  std::cout << dir.x << " " << dir.y << " " << dir.z << std::endl;
  dir.normalize();
  std::cout << dir.x << " " << dir.y << " " << dir.z << std::endl;
  torque += pow(theta / theta_max, 4) * dir;
  if (time_step > 21) {
    std::cout << "dir:" << " " << dir.x << " " << dir.y << " " << dir.z << std::endl;
    std::cout << "theta: " << theta << std::endl;
    std::cout << "torque:" << " " << torque.x << " " << torque.y << " " << torque.z << std::endl;
  }

  if (time_step > 3) {

    // move particles and boat from collisions
    for (int i = 0; i < water_points->size(); i++) {
      WaterPoint *p = (*water_points)[i];
      if (p->isBoat) {
        continue;
      }
      point_t point;
      point = {(*water_points)[i]->next_position.x, (*water_points)[i]->next_position.y,
               (*water_points)[i]->next_position.z};
      auto neighbors_ = neighbor_tree.neighborhood_points(point, 1.5 * particle_dist);

      std::vector<Vector3D> neighbor_positions;
      for (int j = 0; j < neighbors_.size(); ++j) {
        neighbor_positions.push_back(Vector3D(neighbors_[j][0], neighbors_[j][1], neighbors_[j][2]));
      }

      std::vector<WaterPoint *> neighbors;
      for (int j = 0; j < neighbors_.size(); ++j) {
        WaterPoint *w = get_waterPoint_from_location_2(neighbor_positions[j]);
        neighbors.push_back(w);
      }

      std::vector<WaterPoint *> boat_neighbors;
      for (int j = 0; j < neighbors.size(); ++j) {
        if (neighbors[j]->isBoat) {
          boat_neighbors.push_back(get_waterPoint_from_location_2(neighbor_positions[j]));
        }
      }

      for (int j = 0; j < boat_neighbors.size(); ++j) {
        Vector3D diff = (boat_neighbors[j]->position - p->position);
        float diff_norm = diff.norm();
        if (diff_norm < h) {
          // move particle
          p->next_position -= diff / diff.norm() * h;

          // move boat
          com_next += 0.0005 * diff / diff.norm() * h;
          torque += 0.02 * cross(p->position - com, -diff / diff.norm());
        }
      }
    }
    for (int i = 0; i < water_points->size(); ++i) {
      (*water_points)[i]->position = (*water_points)[i]->next_position;
    }
    com = com_next;
  }

  //move particle from plane collision
  for (int i = 0; i < water_points->size(); i++) {
    WaterPoint *p = (*water_points)[i];
    if (p->isBoat) {
      continue;
    }
    if (p->position.x < min_x) {
      p->next_position.x = min_x + .001;
      p->last_position.x = min_x;

    }
    if (max_x < p->position.x) {
      p->next_position.x = max_x - .001;
      p->last_position.x = max_x;

    }
    if (p->position.y < min_y) {
      p->next_position.y = min_y + .001;
      p->last_position.y = min_y;
    }
    if (max_y < p->position.y) {
      p->next_position.y = max_y - .001;
      p->last_position.y = max_y;

    }
    if (p->position.z < min_z) {
      p->next_position.z = min_z + .001;
      p->last_position.z = min_z;

    }
    if (max_z < p->position.z) {
      p->next_position.z = max_z - .001;
      p->last_position.z = max_z;

    }
  }
  for (int i = 0; i < water_points->size(); ++i) {
    (*water_points)[i]->position = (*water_points)[i]->next_position;
  }

  // redo tree and map since they might have changed from past movement
  populate_tree_2(*water_points);
  create_map_2(*water_points);

  // move particles from water
  for (int i = 0; i < water_points->size(); i++) {
    if (i % 1000 == 0) {
      std::cout << (float) i / water_points->size() * 100. << "% done, time step" << time_step << std::endl;
    }

    WaterPoint *p = (*water_points)[i];
    if (p->isBoat) {
      continue;
    }
    point_t point;
    point = {(*water_points)[i]->position.x, (*water_points)[i]->position.y,
             (*water_points)[i]->position.z};
    auto neighbors_ = neighbor_tree.neighborhood_points(point, h);
    std::vector<Vector3D> neighbor_positions;
    for (int j = 0; j < neighbors_.size(); ++j) {
      neighbor_positions.push_back(Vector3D(neighbors_[j][0], neighbors_[j][1], neighbors_[j][2]));
    }
    std::vector<WaterPoint *> neighbors;
    for (int j = 0; j < neighbors_.size(); ++j) {
      WaterPoint *w = get_waterPoint_from_location_2(neighbor_positions[j]);
      neighbors.push_back(w);
    }
    std::vector<WaterPoint*> water_neighbors;
    for (int j = 0; j < neighbors.size(); ++j) {
      if (!neighbors[j]->isBoat) {
        water_neighbors.push_back(get_waterPoint_from_location_2(neighbor_positions[j]));
      }
    }
    std::vector<Vector3D> water_neighbor_positions;
    for (int j = 0; j < water_neighbors.size(); ++j) {
      water_neighbor_positions.push_back(water_neighbors[j]->position);
    }

    // move particle from water collisions
    //std::cout << "starting new point WRRRRRRRRRRY" << std::endl;
    //std::cout << "point: " << p->position.x << " " << p->position.y << " " << p->position.z << std::endl;
    /*
    for (int j = 0; j < water_neighbor_positions.size(); ++j) {
      std::cout << "neighbor:" << water_neighbor_positions[j].x << " " <<
                water_neighbor_positions[j].y << " " << water_neighbor_positions[j].z << std::endl;
    }
    */
    //Vector3D delta = delta_p(i, water_neighbor_positions);
    //std::cout << "delta value:" << std::to_string(delta.x) << " " << std::to_string(delta.y) << " " << std::to_string(delta.z) << std::endl;
    p->next_position += delta_p(i, water_neighbor_positions);
  }
  for (int i = 0; i < water_points->size(); ++i) {
    (*water_points)[i]->position = (*water_points)[i]->next_position;
    (*water_points)[i]->last_position = .6*((*water_points)[i]->last_position) + .4*((*water_points)[i]->position);
  }

  for (int i = 0; i < water_points->size(); i++) {
    WaterPoint *p = (*water_points)[i];
    if (p->isBoat) {
      continue;
    }
    if (p->position.x < min_x) {
      p->next_position.x = min_x + .001;
      p->last_position.x = min_x;

    }
    if (max_x < p->position.x) {
      p->next_position.x = max_x - .001;
      p->last_position.x = max_x;

    }
    if (p->position.y < min_y) {
      p->next_position.y = min_y + .001;
      p->last_position.y = min_y;
    }
    if (max_y < p->position.y) {
      p->next_position.y = max_y - .001;
      p->last_position.y = max_y;

    }
    if (p->position.z < min_z) {
      p->next_position.z = min_z + .001;
      p->last_position.z = min_z;

    }
    if (max_z < p->position.z) {
      p->next_position.z = max_z - .001;
      p->last_position.z = max_z;

    }
  }
  for (int i = 0; i < water_points->size(); ++i) {
    (*water_points)[i]->position = (*water_points)[i]->next_position;
    //std::cout << (*water_points)[i]->position.x << " " << (*water_points)[i]->position.y  << " " << (*water_points)[i]->position.z  << std::endl;
  }

  /*
  // SIMULATE BUOYANT FORCE BETWEEN BOAT POINTS AND WATER POINTS
  //
  // first apply gravity to boat 
  for (int i = 0; i < water_points->size(); i++) {
      WaterPoint *point = ((*water_points)[i];
      if (p->isBoat) {
          p->next_position = 2. * p->position - p->last_position + Vector3D(0., -10., 0.) * dt * dt;
      }
  }
  */


  return;

}
