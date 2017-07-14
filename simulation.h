class PhysicalState {
 public:
  PhysicalState(
      int size_x,
      int size_y
      );
 double& operator()(int i, int j);
 void Print();
 int size_x_;
 int size_y_;
 private:
  double** data_;
};
class Simulation {
 public:
  Simulation(
      int size_x,
      int size_y
      );
  void Run();
  int Step();
 private: 
  PhysicalState physical_state_;
};
