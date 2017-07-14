#include<physical_state.h>
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
  int size_x_;
  int size_y_;
};
