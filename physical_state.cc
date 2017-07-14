#include<physical_state.h>
#include<iostream>
#include<iomanip>
Field::Field(
    int size_x,
    int size_y
    ) : 
    size_x_(size_x),
    size_y_(size_y)
    {
  data_=new double*[size_x];
  for (int i = size_x-1; i>=0; --i) {
    data_[i] = new double[size_y];
  }
  for (int i = 0; i<size_x; ++i){
    for (int j = 0; j<size_y; ++j){
      data_[i][j] = 0.;
    }
  }
}

Field::~Field() {
  for (int i = 0; i<size_x_; ++i) {
    delete[] data_[i];
  }
  delete[] data_;
}

void Field::Print(std::ostream& stream) {
  for (int i = 0; i<size_x_; ++i){
    for (int j = 0; j<size_y_; ++j){
      stream<<std::setprecision(3)<<std::fixed<<std::setw(6)<<data_[i][j]<<" ";
    }
    stream<<'\n';
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
  double D1 = 0.001;
  double D2 = 0.001;
  double dx = 0.2;
  double F = 0.065;
  double k = 0.03;
  double dt = 1.;
  PhysicalState new_state(size_x_, size_y_);
  for(int i = 0; i<size_x_; ++i){
    for(int j = 0; j<size_y_; ++j){
      nu(i,j) = u_(i, j) + D1/(dx*dx)*dt*(u_(i+1, j)+u_(i-1, j)+u_(i, j+1)+u_(i, j-1)-4*u_(i, j)) - u_(i,j)*v_(i,j)*v_(i,j) + F*(1-u_(i,j));
      nv(i,j) = v_(i, j) + D2/(dx*dx)*dt*(v_(i+1, j)+v_(i-1, j)+v_(i, j+1)+v_(i, j-1)-4*v_(i, j)) + u_(i,j)*v_(i,j)*v_(i,j) - (F+k)*v_(i,j);
      nw(i,j) = 0;
    }
  }
  for(int i = 0; i<size_x_; ++i){
    for(int j = 0; j<size_y_; ++j){
      u_(i,j) = nu(i,j);
      v_(i,j) = nv(i,j);
      w_(i,j) = nw(i,j);
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
