class Simulation {
 public:
  Simulation(
      int size_x,
      int size_y
      );
  void Run();
 private: 
  double** physical_state_; //TODO move to separate class, add smart ownership to avoid data leakage
};
