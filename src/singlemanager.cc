#include<singlemanager.h>
#include<simulation.h>
#include<analyser.h>
#include<iostream>
#include<memory>

void SingleFingerSimulation::InitValues() {
  Simulation::InitValues();
  physical_state_->v_.SetValue(0.0);
  physical_state_->u_.SetValue(0.1);
}

void SingleFingerSimulation::Ignite() {
  physical_state_->v_.SetValuePart(0.6, 0, 20, size_y_/2-5, size_y_/2+5);
  ignited_=true;
}

void SingleFingerSimulation::ApplyBoundaryConditions() {
  Simulation::ApplyBoundaryConditions();
  if (!ignited_) {
    new_state_->u_.SetValuePart(0., 0, 20, size_y_/2-5, size_y_/2+5);
  }
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
  simulation_ = std::unique_ptr<SingleFingerSimulation>( new SingleFingerSimulation(
        400,
        200,
        save_steps_,
        max_step_,
        data_out_,
        12,
        0.5));
  analyser_ = std::unique_ptr<Analyser>( new Analyser(
        communication_stream_,
        analyser_out_
        ));
}

void SingleManager::Run() {
  simulation_->InitValues();
  while(current_step_ < max_step_ && !analyser_->front_reached_end_) {
    Step();
  }
}

void SingleManager::Step() {
  std::cout<<"current step:"<<current_step_<<std::endl;
  simulation_->Steps(save_steps_);
  current_step_ = simulation_->current_step_;
  if (current_step_ > change_step_) {
    simulation_->PrintData(communication_stream_);
    communication_stream_<<std::flush;
    std::cout<<"new data. Performing analysis..."<<std::endl;
    analyser_->Step();
    if (analyser_->front_reached_end_) {
      std::cout<<"SM:front-end signal from analyser, ending sim"<<std::endl;
    }
  } else if (current_step_ == change_step_) {
    simulation_->Ignite();
    std::cout<<"change step"<<std::endl;
    save_steps_ = after_change_step_;
  }
}
