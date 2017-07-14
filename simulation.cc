#include<simulation.h>
#include<iostream>
PhysicalState::PhysicalState(
    int size_x,
    int size_y
    ) : 
    size_x_(size_x),
    size_y_(size_y)
    {
  data_=new double*[size_x];
  for (auto i = size_x-1; i>=0; --i) {
    data_[i] = new double[size_y];
  }
  for(int i = 0; i<size_x; ++i){
    for(int j = 0; j<size_y; ++j){
      data_[i][j] = 0.;
    }
  }
}

double& PhysicalState::operator()(int i, int j){
    return data_[i][j];
}

void PhysicalState::Print() {
  for(int i = 0; i<size_x_; ++i){
    for(int j = 0; j<size_y_; ++j){
      std::cout<<data_[i][j]<<" ";
    }
    std::cout<<'\n';
  }
}

Simulation::Simulation(
    int size_x,
    int size_y
    ) : 
    physical_state_(size_x, size_y)
    {
  physical_state_.Print();
  physical_state_(1,1)= 19.;
  physical_state_.Print();
}


void Simulation::Run() {
  std::cout<<"Hello, there!"<<std::endl;
}

int Simulation::Step() {
}
  
