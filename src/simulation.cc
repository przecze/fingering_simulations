#include<simulation.h>
#include<iostream>
#include<future>
#include<cmath>

Simulation::Simulation(
    int size_x,
    int size_y,
    int steps_total,
    int big_step,
    std::ostream& stream,
    int threads_number,
    double Pe
    ) : 
    size_x_(size_x),
    size_y_(size_y),
    steps_total_(steps_total),
    big_step_(big_step),
    out_stream_(stream),
    threads_number_(threads_number),
    physical_state_(new PhysicalState (size_x, size_y)),
    new_state_(new PhysicalState(size_x, size_y)),
    last_time_stamp_(0),
    Pe(Pe)
    {
  int x_end = 0;
  for (int i = 0; i<threads_number_-1; ++i) {
    x_end+=size_x_/threads_number;
    ranges_for_threads_.push_back(x_end);
  }
  ranges_for_threads_.push_back(size_x); 
  using namespace std::chrono;
  last_time_stamp_ = duration_cast< milliseconds >(
      system_clock::now().time_since_epoch()).count();
}


void Simulation::Run() {
  std::cout<<"Running simulation!"<<std::endl;
  TimeStamp();
  InitValues();
  for (int step_no = 0; step_no<steps_total_; ++step_no) {
    if (big_step_!=-1 && !(step_no%big_step_)) {
      PrintData(out_stream_);
      std::cout<<"Step "<<step_no<<" / "<<steps_total_<<std::endl;
      TimeStamp();
    }
    Step();
  }
  TimeStamp();
  PrintData(out_stream_);
}

void Simulation::PrintData(std::ostream& os) {
  os<<"step "<<current_step_<<'\n';
  physical_state_->Print(os);
}

void Simulation::Steps(int steps) {
  while (steps --) {
    Step();
    ++current_step_;
  }
  if (!ignited_) {
    //idt = dt - 0.95* (dt - idt); //idt grows up to dt
    //std::cout<<"new idt: "<<idt<<std::endl;
  }
  TimeStamp();
  PrintData(out_stream_);
}

int Simulation::Step() {
  int x_begin = 0;
  int x_end = 0;
  std::vector<std::thread> threads;
  for(int i = 0; i<threads_number_; ++i){
    x_begin = x_end;
    x_end = ranges_for_threads_[i];
    threads.push_back(std::thread([x_begin, x_end, this](){
        this->PartialStepCalculation(x_begin, x_end);
        }));
  }
  for(auto& thr : threads){
    thr.join();
  }
  ApplyBoundaryConditions();
  SwapOldAndNewState();
  return 0;
}

void Simulation::PartialStepCalculation(int x_begin, int x_end) {

  Field& U(physical_state_->u_);
  Field& V(physical_state_->v_);
  Field& W(physical_state_->w_);
  Field& NU(new_state_->u_);
  Field& NV(new_state_->v_);
  Field& NW(new_state_->w_);
  
  if (ignited_) { 
    for (int i = x_begin; i<x_end; ++i) {
      for (int j = 0; j<size_y_; ++j) {

        const double u = U(i,j);
        const double v = V(i,j);
        const double w = W(i,j);
        const double f = (v>vp?1:0)*u*theta*exp(theta-theta/v);

        NU(i,j) = u + 
            dt*(
              (1./phi)*(1./Le)*(1./(dx*dx))*(U(i+1, j)+U(i-1, j)+U(i, j+1)+U(i, j-1)-4*U(i, j))
                - gamma*f/phi
                - Pe*(1/(dx*2.0))*(U(i-1,j) - U(i+1,j))
                );

        NV(i,j) = v +
            dt*(
                1./(dx*dx)*(V(i+1, j)+V(i-1, j)+V(i, j+1)+V(i, j-1)-4*V(i, j))
                +  beta*gamma*f
                //-Pe*phi*lam/dx/2.0*(V(i-1,j)-V(i+1,j))
                -ha*v
                );
        NW(i,j) = (v>vp?0:w);

      }
    }
  } else {
    for (int i = x_begin; i<x_end; ++i) {
      for (int j = 0; j<size_y_; ++j) {
        const double u = U(i,j);
        NU(i,j) = u + 
            idt*(
              (1./phi)*(1./Le)*(1./(dx*dx))*(U(i+1, j)+U(i-1, j)+U(i, j+1)+U(i, j-1)-4*U(i, j))
                - Pe*(1/(dx*2.0))*(U(i-1,j) - U(i+1,j))
                );
      }
    }
  }
}


void Simulation::ApplyBoundaryConditions() {
  double K = Pe*Le*phi*dx;
  for(int j = 0; j<size_y_; ++j){
    new_state_->u_(size_x_-1, j) = (K + new_state_->u_(size_x_-2,j))/(1+K);
    new_state_->u_(0, j) = new_state_->u_(1,j);
  }
  if (current_step_<10) {
  }
  new_state_->v_.SetValuePart(0., 0, 1, 0, size_y_);
  new_state_->v_.SetValuePart(0., size_x_-1, size_x_, 0, size_y_);
}

void Simulation::InitValues() {
  //for (int i = 0; i<size_x_; ++i) {
  //  physical_state_->u_.SetValuePart(1-std::exp(-Le*phi*dx*i*Pe), i, i+1, 0, size_y_);
  //}
  physical_state_->u_.SetValue(1.);
  physical_state_->v_.SetValue(0);
  physical_state_->w_.SetValue(1.);
  PrintData(out_stream_);
}

void Simulation::Ignite() {
  //physical_state_->u_.SetRandomPart(u0, 0.0, 20, 30, 0, size_y_);
  physical_state_->v_.SetRandomPart(0, 1., 3, 10, 0, size_y_);
  ignited_ = true;
}



void Simulation::SwapOldAndNewState(){
  physical_state_.swap(new_state_);
}

void Simulation::TimeStamp(){
  using namespace std::chrono;
  //int time_difference = duration_cast< milliseconds >(
  //    system_clock::now().time_since_epoch()).count() - last_time_stamp_;
  
  int new_time_stamp = duration_cast< milliseconds >(
      system_clock::now().time_since_epoch()).count();
  std::cout<<"Simulation::TimeStamp :"
           <<"step: "<<current_step_<<", "
           <<" threads:" <<threads_number_<<", "
           <<"Pe: "<<Pe<<", "
           <<(new_time_stamp-last_time_stamp_)*0.001
           <<"s since last"<<std::endl
           ;
  last_time_stamp_ = new_time_stamp;
}
