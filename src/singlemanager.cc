#include<singlemanager.h>
#include<simulation.h>
#include<analyser.h>
#include<iostream>
#include<memory>

void SingleFingerSimulation::InitValues() {
  Simulation::InitValues();
  physical_state_->v_.SetValue(0.0);

  physical_state_->u_.SetValuePart(0.6, 0, 20, size_y_/2-10, size_y_/2+10);
}


SingleManager::SingleManager(
      std::ostream& output,
      std::ostream& analyser_out) :
  data_out_(output),
  analyser_out_(analyser_out),
  communication_stream_()
  {
}

void SingleManager::Init() {
    simulation_ = std::unique_ptr<Simulation>( new Simulation(
          500,
          500,
          save_steps_,
          max_step_,
          data_out_,
          5,
          0.3));
    analyser_ = std::unique_ptr<Analyser>( new Analyser(
          communication_stream_,
          analyser_out_
          ));
}

void SingleManager::Run() {
  simulation_->InitValues();
  while(current_step_ < max_step_) {
    Step();
  }
}

void SingleManager::Step() {
  std::cout<<"current step:"<<current_step_;
  simulation_->Steps(save_steps_);
  current_step_ = simulation_->current_step_;
  if (current_step_ > 400) {
    simulation_->PrintData(communication_stream_);
    communication_stream_<<std::flush;
    std::cout<<"new data. Performing analysis..."<<std::endl;
    analyser_->Step();
  } else if (current_step_ == 30000) {
    std::cout<<"change step"<<std::endl;
    save_steps_ = 1000;
  }
}
