#include<physical_state.h>
#include<iostream>
#include<iomanip>
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
    return data_[(i+size_x_)%size_x_][(j+size_y_)%size_y_];
}

void PhysicalState::Print() {
  for(int i = 0; i<size_x_; ++i){
    for(int j = 0; j<size_y_; ++j){
      std::cout<<std::setprecision(2)<<std::fixed<<std::setw(4)<<data_[i][j]<<" ";
    }
    std::cout<<'\n';
  }
}
