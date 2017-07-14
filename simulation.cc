#include<simulation.h>
#include<iostream>
Simulation::Simulation(
      int size_x,
      int size_y
      ) {
  physical_state_=new double*[size_x];
  for (auto i = size_x-1; i>=0; --i) {
    physical_state_[i] = new double[size_y];
  }
}
void Simulation::Run() {
  std::cout<<"Hello, there!"<<std::endl;
}
