#ifndef SINGLEMANAGER_H
#define SINGLEMANAGER_H
#include<sstream>
#include<iostream>
#include<memory>
#include<simulation.h>
#include<analyser.h>

class SingleFingerSimulation : public Simulation {
 public:
  using Simulation::Simulation;
  void InitValues();
  void Ignite();
  void ApplyBoundaryConditions();
  int ignition_x  = 5;
  int ignition_w = 4;
};

class OxygenOnlySimulation : public Simulation {
 public:
  using Simulation::Simulation;
  void Ignite();
};

class SingleManager {
 public:
  SingleManager(
      std::ostream& output,
      std::ostream& analyser_out,
      double Pe
      ); 
  void Run();
  void Init();
 private:
  std::unique_ptr<Simulation> simulation_;
  std::unique_ptr<Analyser> analyser_;
  std::stringstream communication_stream_;
  std::ostream& data_out_;
  std::ostream& analyser_out_;
  void Step();
  int current_step_ = 0;
  int max_step_ = 400000;
  int save_steps_ = 5000;
  int change_step_ = 5000;
  int after_change_step_ = 5000;
  double Pe = 0.3;
};

#endif
