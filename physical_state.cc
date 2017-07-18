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

void Field::Set(double value) {//TODO implement using memset
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

void Field::SetRandom(double base, double multiplier, int x1, int x2, int y1, int y2) {
  for(int i = x1; i<x2; ++i){
    for(int j = y1; j<y2; ++j){
      data_[i][j] = base + multiplier*Random();
    }
  }
} 

void Field::Set(double value, int x1, int x2, int y1, int y2) {
  for(int i = x1; i<x2; ++i){
    for(int j = y1; j<y2; ++j){
      data_[i][j] = value;
    }
  }
} 

void Field::Set(Field& field) {//TODO implement using memset
  for(int i = 0; i<size_x_; ++i){
    for(int j = 0; j<size_y_; ++j){
      data_[i][j] = field(i,j);
    }
  }
}  

void Field::PartialSet(Field& field, int x_begin, int x_end) {//TODO implement using memset
  for(int i = x_begin; i<x_end; ++i){
    for(int j = 0; j<size_y_; ++j){
      data_[i][j] = field(i,j);
    }
  }
}  

inline double& Field::operator()(int i, int j){
  if(i>=0 && i<size_x_ && j>=0 && j<size_y_)
    return data_[i][j];
  else
    return data_[(i+size_x_)%size_x_][(j+size_y_)%size_y_];
}

PhysicalState::PhysicalState (
    int size_x,
    int size_y
    ) : 
    size_x_(size_x),
    size_y_(size_y),
    u_(new Field(size_x, size_y)),
    v_(new Field(size_x, size_y)),
    w_(new Field(size_x, size_y)),
    nu_(new Field(size_x, size_y)),
    nv_(new Field(size_x, size_y)),
    nw_(new Field(size_x, size_y))
    {
}
    
void PhysicalState::PartialStepCalculation(int x_begin, int x_end) {
  double dt = 0.0025;
  double dx = 0.5   ;
  double D1 = 1.    ;
  double D2 = 1.    ;
  double theta=1.94;
  double beta=20.  ;
  double gamma=5.  ;
  double ha=0.328  ;
  double phi=0.458 ;
  double w0=0.1    ;
  double v0=1.0    ;
  double sigma=0.02;
  double lam=0.0   ;
  double haw=1.0   ;
  double Pe=.4     ;
  double vp=0.45   ;
  double Le=0.1    ;

  Field& U(*u_);
  Field& NU(*nu_);
  Field& V(*v_);
  Field& NV(*nv_);
  Field& W(*w_);
  Field& NW(*nw_);
  
  
  for (int i = x_begin; i<x_end; ++i) {
    for (int j = 0; j<size_y_; ++j) {

      register double u = U(i,j);
      register double v = V(i,j);
      register double w = W(i,j);
      register double f = gamma*(v>vp?1:0)*(w>0?1:0)*u*theta*exp(theta-theta/v);

      NU(i,j) = u + 
          dt*(
              (1./(phi*Le*dx*dx))*(U(i+1, j)+U(i-1, j)+U(i, j+1)+U(i, j-1)-4*U(i, j))
              - f/phi
              - Pe/dx/2.0*(U(i+1,j) - U(i-1,j))
              );

      NV(i,j) = v +
          dt*(
              0.3/(dx*dx)*(V(i+1, j)+V(i-1, j)+V(i, j+1)+V(i, j-1)-4*V(i, j))
              +  beta*f
              -Pe*phi*lam/dx/2.0*(V(i+1,j)-V(i-1,j))
              -ha*(v-sigma)
              );
      register double new_w = w - haw*dt*f;
      NW(i,j) = (new_w>0?new_w:0);

    }
  }
}

void PhysicalState::ApplyBoundaryConditions() {
  u_->Set(0., 0, 1, 0, size_y_);
  u_->Set(0.1, size_x_-1, size_x_, 0, size_y_);
  v_->Set(1., 0, 1, 0, size_y_);
  v_->Set(0., size_x_-1, size_x_, 0, size_y_);
}

void PhysicalState::SwapFieldsWithNew() {
  u_.swap(nu_);
  v_.swap(nv_);
  w_.swap(nw_);

}

void PhysicalState::InitValues() {
  u_->Set(0.1);
  u_->Set(0., 0, 1, 0, size_y_);
  u_->Set(1, size_x_-1, size_x_, 0, size_y_);
  u_->SetRandom(0.0, 0.1, 0, 10, 0, size_y_);

  v_->Set(0);
  v_->SetRandom(0, 1., 0, 10, 0, size_y_);

  w_->Set(1.);
}

void PhysicalState::Print(std::ostream& stream) {
  stream<<"u\n";
  u_->Print(stream);
  stream<<"v\n";
  v_->Print(stream);
  stream<<"w\n";
  w_->Print(stream);
  stream<<'\n';
}
