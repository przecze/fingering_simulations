#include<singlemanager.h>
#include<simulation.h>
#include<analyser.h>
#include<iostream>
#include<memory>


void SingleFingerSimulation::Ignite() {
  physical_state_->v_.SetValuePart(1.,
              ignition_x/3,          ignition_x,
               size_y_/2-ignition_w, size_y_/2+ignition_w);
  physical_state_->v_.SetValuePart(0.5,
               0,          ignition_x/3,
               0, size_y_);
  ignited_=true;
  std::cout<<"Ignited"<<std::endl;
}

void SingleFingerSimulation::InitValues() {
  Simulation::InitValues();
  Field& U = physical_state_->u_;
  //for (int i = 0; i<size_x_; ++i) {
  //  U.SetValuePart(1-1*std::exp(-Le*phi*dx*i*Pe), i, i+1, 0, size_y_);
  //}
  for (int x = 0; x<size_x_; ++x) {
    for (int y = 0; y<size_y_; ++y) {
      U(x,y) = (1 - 0*std::exp(-1*Le*phi*dx*Pe*std::min(std::sqrt(
          (x-ignition_x)*(x-ignition_x) + (y - size_y_/2)*(y-size_y_/2)),
          double(x))));
    }
  }
        
}

void SingleFingerSimulation::ApplyBoundaryConditions() {
  //if (!ignited_) {
  //  new_state_->u_.SetValuePart(0.1, std::max(ignition_x -5, 0), ignition_x,
  //        size_y_/2-ignition_w/2, size_y_/2+ignition_w/2);
  //  //new_state_->u_.SetValuePart(0.15, 1, 2, 0, size_y_);
  //}
  Simulation::ApplyBoundaryConditions();
}

void OxygenOnlySimulation::Ignite() {
}

SingleManager::SingleManager(
      std::ostream& output,
      std::ostream& analyser_out,
      double Pe
      ) :
  data_out_(output),
  analyser_out_(analyser_out),
  Pe(Pe),
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
        15,
        Pe));
  analyser_ = std::unique_ptr<Analyser>( new Analyser(
        communication_stream_,
        analyser_out_,
        Analyser::kMetaData
        ));
}

void SingleManager::Run() {
  simulation_->InitValues();
  while(current_step_ < max_step_ && !analyser_->simulation_ended_) {
    Step();
  }
  if ( !analyser_->simulation_ended_) {
    analyser_->OnEnd();
  }
}

void SingleManager::Step() {
  simulation_->Steps(save_steps_);
  current_step_ = simulation_->current_step_;
  if (current_step_ > change_step_ + 3*after_change_step_) {
    simulation_->PrintData(communication_stream_);
    communication_stream_<<std::flush;
    std::cout<<"new data. Performing analysis..."<<std::endl;
    analyser_->Step();
    if (analyser_->simulation_ended_) {
      std::cout<<"SM(Pe: "<<Pe<<") end signal from analyser, ending sim"<<std::endl;
    }
  } else if (current_step_ == change_step_) {
    simulation_->Ignite();
    std::cout<<"Ignited"<<std::endl;
    save_steps_ = after_change_step_;
    std::cout<<"change step"<<std::endl;
  }
}
