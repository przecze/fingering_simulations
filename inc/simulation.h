#ifndef SIMULATION_H
#define SIMULATION_H
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
      int threads_number = 1,
      double Pe = 0.3
      );
  void Run();
  void Steps(int);
  void InitValues();
  void PrintData(std::ostream&);
  int Step();
  int current_step_ = 0;
 protected: 
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
  virtual void ApplyBoundaryConditions();
  void SwapOldAndNewState();
  void TimeStamp();
  const double Pe;
  constexpr static double dt = 0.0025/0.3;
  constexpr static double dx = 0.5/0.3   ;
  constexpr static double theta=1.94;
  constexpr static double beta=20. ;
  constexpr static double gamma=5.*0.3  ;
  constexpr static double ha=0.328*0.3  ;
  constexpr static double phi=0.458 ;
  constexpr static double sigma=0.02;
  constexpr static double lam=0.0   ;
  constexpr static double vp=0.45   ;
  constexpr static double Le=0.1*0.3    ;
};
#endif
