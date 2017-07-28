#include<physical_state.h>
#include<fstream>
#include<iostream>
#include<vector>
class Simulation {
 public:
  Simulation(
      int size_x,
      int size_y,
      int steps_total,
      int big_step,
      std::ostream& stream = std::cout,
      int threads_number = 1
      );
  void Run();
  int Step();
 private: 
  std::ostream& out_stream_;
  std::unique_ptr<PhysicalState> physical_state_;
  std::unique_ptr<PhysicalState> new_state_;
  int size_x_;
  int size_y_;
  int steps_total_;
  int big_step_;
  int threads_number_;
  int last_time_stamp_;
  std::vector<int> ranges_for_threads_;
  void PartialStepCalculation(int x_begin, int x_end);
  void InitValues();
  void ApplyBoundaryConditions();
  void SwapOldAndNewState();
  void TimeStamp();
  constexpr static double Pe = 0.3;
};
