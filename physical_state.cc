#include<physical_state.h>
#include<iostream>
#include<iomanip>
#include<cstdlib>
#include<cmath>

double Random() {
  return double(rand())/RAND_MAX;
}

Field::Field(
    int size_x,
    int size_y
    ) : 
    size_x_(size_x),
    size_y_(size_y)
    {
  data_=new double*[size_x];
  for (int i = size_x-1; i>=0; --i) {
    data_[i] = new double[size_y]();
  }
  
}

Field::~Field() {
  for (int i = 0; i<size_x_; ++i) {
    delete[] data_[i];
  }
  delete[] data_;
}

void Field::Print(std::ostream& stream) {
  stream<<size_x_<<" "<<size_y_<<'\n';
  for (int i = 0; i<size_x_; ++i) {
    for (int j = 0; j<size_y_; ++j) {
      stream<<std::setprecision(4)<<std::fixed<<std::setw(6)<<data_[i][j]<<" ";
    }
    stream<<'\n';
  }
}

void Field::SetValue(double value) {//TODO implement using memset
  for(int i = 0; i<size_x_; ++i){
    for(int j = 0; j<size_y_; ++j){
      data_[i][j] = value;
    }
  }
} 


void Field::SetRandomPart(double base, double multiplier, int x1, int x2, int y1, int y2) {
  for(int i = x1; i<x2; ++i){
    for(int j = y1; j<y2; ++j){
      data_[i][j] = base + multiplier*Random();
    }
  }
} 

void Field::SetValuePart(double value, int x1, int x2, int y1, int y2) {
  for(int i = x1; i<x2; ++i){
    for(int j = y1; j<y2; ++j){
      data_[i][j] = value;
    }
  }
} 


PhysicalState::PhysicalState (
    int size_x,
    int size_y
    ) : 
    size_x_(size_x),
    size_y_(size_y),
    u_(size_x, size_y),
    v_(size_x, size_y),
    w_(size_x, size_y)
    {
}

void PhysicalState::Print(std::ostream& stream) {
  stream<<"u\n";
  u_.Print(stream);
  stream<<"v\n";
  v_.Print(stream);
  stream<<"w\n";
  w_.Print(stream);
  stream<<'\n';
}
