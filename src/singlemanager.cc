#include<singlemanager.h>
#include<simulation.h>
#include<analyser.h>
#include<iostream>
#include<memory>


void SingleFingerSimulation::Ignite() {
  physical_state_->v_.SetValuePart(1,  55, 60, size_y_/2-1, size_y_/2+1);
  ignited_=true;
}

void SingleFingerSimulation::InitValues() {
  Simulation::InitValues();
  for (int i = 0; i<size_x_; ++i) {
    physical_state_->u_.SetValuePart(1-std::exp(-Le*phi*dx*i*Pe), i, i+1, 0, size_y_);
  }
}

void SingleFingerSimulation::ApplyBoundaryConditions() {
  if (!ignited_) {
    new_state_->u_.SetValuePart(0.05, 55, 60, size_y_/2-1, size_y_/2+1);
    //new_state_->u_.SetValuePart(0.15, 1, 2, 0, size_y_);
  }
  Simulation::ApplyBoundaryConditions();
}

void OxygenOnlySimulation::Ignite() {
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
  simulation_ = std::unique_ptr<Simulation>( new SingleFingerSimulation(
        200,
        200,
        save_steps_,
        max_step_,
        data_out_,
        8,
        0.1));
  analyser_ = std::unique_ptr<Analyser>( new Analyser(
        communication_stream_,
        analyser_out_
        ));
}

void SingleManager::Run() {
  simulation_->InitValues();
  while(current_step_ < max_step_) {// && !analyser_->simulation_ended_) {
    Step();
  }
}

void SingleManager::Step() {
  std::cout<<"current step:"<<current_step_<<std::endl;
  simulation_->Steps(save_steps_);
  current_step_ = simulation_->current_step_;
  if (current_step_ > change_step_ + 3*after_change_step_) {
    simulation_->PrintData(communication_stream_);
    communication_stream_<<std::flush;
    std::cout<<"new data. Performing analysis..."<<std::endl;
    analyser_->Step();
    if (analyser_->simulation_ended_) {
      std::cout<<"SM: end signal from analyser, ending sim"<<std::endl;
    }
  } else if (current_step_ == change_step_) {
    simulation_->Ignite();
    std::cout<<"change step"<<std::endl;
    save_steps_ = after_change_step_;
  }
}
