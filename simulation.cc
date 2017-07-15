#include<simulation.h>
#include<iostream>
#include<fstream>

Simulation::Simulation(
    int size_x,
    int size_y,
    std::ostream& stream
    ) : 
    size_x_(size_x),
    size_y_(size_y),
    out_stream_(stream),
    physical_state_(size_x, size_y)
    {
}


void Simulation::Run() {
  std::cout<<"Hello, there!"<<std::endl;
  physical_state_.u_.Set(1.);
  physical_state_.u_.Set(0.4, 0.2, size_x_/4, 3*size_x_/4, size_y_/4, 3*size_y_/4);
  physical_state_.v_.Set(0.2, 0.2, size_x_/4, 3*size_x_/4, size_y_/4, 3*size_y_/4);
  for (int step_no = 0; step_no<1000; ++step_no) {
    if (!(step_no%100)) {
      out_stream_<<"step "<<step_no<<'\n';
      physical_state_.Print(out_stream_);
      std::cout<<'#'<<std::flush;
    }
    Step();
  }
  std::cout<<std::endl;
  out_stream_<<"step "<<1000<<'\n';
  physical_state_.Print(out_stream_);
  
}

int Simulation::Step() {
  physical_state_.Step();
  return 0;
}
  
