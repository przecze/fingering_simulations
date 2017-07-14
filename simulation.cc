#include<simulation.h>
#include<iostream>

Simulation::Simulation(
    int size_x,
    int size_y
    ) : 
    size_x_(size_x),
    size_y_(size_y),
    physical_state_(size_x, size_y)
    {
}


void Simulation::Run() {
  std::cout<<"Hello, there!"<<std::endl;
  physical_state_(5,5)= 19.;
  for(int step_no = 0; step_no<5; ++step_no){
    std::cout<<"step no "<<step_no<<std::endl;
    physical_state_.Print();
    std::cout<<std::endl;
    Step();
  }
  
}

int Simulation::Step() {
  double D1 = 0.001;
  double dt = 1.;
  PhysicalState new_state(size_x_, size_y_);
  std::cout<<"step"<<std::endl;
  for(int i = 0; i<size_x_; ++i){
    for(int j = 0; j<size_y_; ++j){
      double new_value = physical_state_(i, j) + D1*dt*(physical_state_(i+1, j)+physical_state_(i-1, j)+physical_state_(i, j+1)+physical_state_(i, j-1)-4*physical_state_(i, j));
      new_state(i,j) = new_value;
    }
  }
  for(int i = 0; i<size_x_; ++i){
    for(int j = 0; j<size_y_; ++j){
      physical_state_(i,j) = new_state(i,j);
    }
  }
  return 0;


}
  
