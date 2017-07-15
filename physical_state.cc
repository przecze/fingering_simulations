#include<physical_state.h>
#include<iostream>
#include<iomanip>
#include<cstdlib>

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
      stream<<std::setprecision(3)<<std::fixed<<std::setw(6)<<data_[i][j]<<" ";
    }
    stream<<'\n';
  }
}

void Field::Set(double value) {
  for(int i = 0; i<size_x_; ++i){
    for(int j = 0; j<size_y_; ++j){
      data_[i][j] = value;
    }
  }
} 

void Field::Set(std::function<double(int,int)> function) {
  for (int i = 0; i<size_x_; ++i) {
    for (int j = 0; j<size_y_; ++j) {
      data_[i][j] = function(i,j);
    }
  }
} 

void Field::Set(double base, double rand_base, int x1, int x2, int y1, int y2) {
  for(int i = x1; i<x2; ++i){
    for(int j = y1; j<y2; ++j){
      data_[i][j] = base + rand_base*Random();
    }
  }
} 

double& Field::operator()(int i, int j){
    return data_[(i+size_x_)%size_x_][(j+size_y_)%size_y_];
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
    
void PhysicalState::Step() {
  Field nu(size_x_, size_y_), nv(size_x_, size_y_), nw(size_x_, size_y_); 
  double D1 = 0.00002;
  double D2 = 0.00001;
  double dx = 0.02;
  double F = 0.037;
  double k = 0.06;
  double dt = 2.;
  for(int i = 0; i<size_x_; ++i){
    for(int j = 0; j<size_y_; ++j){
      nu(i,j) =
          u_(i, j)
          + dt*(
              D1/(dx*dx)*(u_(i+1, j)+u_(i-1, j)+u_(i, j+1)+u_(i, j-1)-4*u_(i, j))
              - u_(i,j)*v_(i,j)*v_(i,j)
              + F*(1-u_(i,j))
              );
      nv(i,j) =
          v_(i, j)
          + dt*(
              D2/(dx*dx)*(v_(i+1, j)+v_(i-1, j)+v_(i, j+1)+v_(i, j-1)-4*v_(i, j))
              + u_(i,j)*v_(i,j)*v_(i,j) 
              - (F+k)*v_(i,j)
              );
    }
  }
  for(int i = 0; i<size_x_; ++i){
    for(int j = 0; j<size_y_; ++j){
      u_(i,j) = nu(i,j);
      v_(i,j) = nv(i,j);
    }
  }
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
