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
};


class SingleManager {
 public:
  SingleManager(
      std::ostream& output,
      std::ostream& analyser_out
      ); 
  void Run();
  void Init();
 private:
  std::unique_ptr<SingleFingerSimulation> simulation_;
  std::unique_ptr<Analyser> analyser_;
  std::stringstream communication_stream_;
  std::ostream& data_out_;
  std::ostream& analyser_out_;
  void Step();
  int current_step_ = 0;
  int max_step_ = 100000000;
  int save_steps_ = 5000;
  int change_step_ = 20000;
  int after_change_step_ = 1000;
};

#endif
