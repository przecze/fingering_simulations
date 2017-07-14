#include<physical_state.h>
#include<fstream>
#include<iostream>
class Simulation {
 public:
  Simulation(
      int size_x,
      int size_y,
      std::ostream& stream = std::cout
      );
  void Run();
  int Step();
 private: 
  std::ostream& out_stream_;
  PhysicalState physical_state_;
  int size_x_;
  int size_y_;
};
