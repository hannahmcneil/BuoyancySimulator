void main() {

  float dt = 0.05;
  int num_time_steps = 100;
  int num_time_steps_per_frame = 1;

  std::vector<Water_Point*> water_points;

  generate_initial_water_positions(water_points);

  for (int i = 0; i < num_time_steps; ++i) {
    if (i % num_time_steps_per_frame == 0) {
      save_dae(water_points, i);
    }
    simulate(water_points, dt);
  }
  save_dae(water_points, num_time_steps);

}