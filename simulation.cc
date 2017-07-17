#include<simulation.h>
#include<iostream>
#include<fstream>
#include<thread>

Simulation::Simulation(
    int size_x,
    int size_y,
    std::ostream& stream,
    int threads_number
    ) : 
    size_x_(size_x),
    size_y_(size_y),
    out_stream_(stream),
    threads_number_(threads_number),
    physical_state_(size_x, size_y),
    last_time_stamp_(0)
    {
  for (int x_end = size_x/threads_number; x_end<size_x; x_end+=size_x/threads_number) {
    ranges_for_threads_.push_back(x_end);
  }
  ranges_for_threads_.push_back(size_x); 
  using namespace std::chrono;
  last_time_stamp_ = duration_cast< milliseconds >(
      system_clock::now().time_since_epoch()).count();
}


void Simulation::Run() {
  std::cout<<"Running simulation!"<<std::endl;
  physical_state_.InitValues();
  int steps = 4000;
  for (int step_no = 0; step_no<steps; ++step_no) {
    if (!(step_no%100)) {
      out_stream_<<"step "<<step_no<<'\n';
      physical_state_.Print(out_stream_);
      std::cout<<"Step "<<step_no<<" / "<<steps<<std::endl;
      TimeStamp();
    }
    Step();
  }
  std::cout<<'|'<<std::endl;
  out_stream_<<"step "<<1000<<'\n';
  physical_state_.Print(out_stream_);
  
}

int Simulation::Step() {
  int x_begin = 0;
  int x_end = 0;
  std::vector<std::thread> threads;
  for(int i = 0; i<threads_number_; ++i){
    x_begin = x_end;
    x_end = ranges_for_threads_[i];
    threads.push_back(std::thread([x_begin, x_end, this](){
        this->physical_state_.PartialStepCalculation(x_begin, x_end);
        }));
  }
  for(int i = 0; i<threads_number_; ++i){
    threads[i].join();
  }
  physical_state_.SwapFieldsWithNew();
  physical_state_.ApplyBoundaryConditions();
  return 0;
}

void Simulation::TimeStamp(){
  using namespace std::chrono;
  //int time_difference = duration_cast< milliseconds >(
  //    system_clock::now().time_since_epoch()).count() - last_time_stamp_;
  
  int new_time_stamp = duration_cast< milliseconds >(
      system_clock::now().time_since_epoch()).count();
  std::cout<<"Simulation::TimeStamp : "<<new_time_stamp<<", "<<new_time_stamp-last_time_stamp_<<" since last time stamp"<<std::endl;
  last_time_stamp_ = new_time_stamp;
}
