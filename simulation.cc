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
  std::cout<<"Running simulation!"<<std::endl;
  physical_state_.InitValues();
  int steps = 4000;
  std::cout<<'|';
  for (int i = 0; i<steps; i+=100) {
    std::cout<<'-';
  }
  std::cout<<"|\n|"<<std::flush;
  for (int step_no = 0; step_no<steps; ++step_no) {
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
  
