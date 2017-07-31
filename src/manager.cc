#include<manager.h>

#include<iostream>
#include<sstream>
#include<iomanip>
#include<fstream>
#include<thread>

#include<simulation.h>
#include<analyser.h>


Manager::Manager(std::vector<double> Pe_values) :
    Pe_values_(Pe_values),
    simulations_num_(Pe_values.size()){
  std::cout<<"manager created"<<std::endl;
  GenerateFileNames();
}

void Manager::Run() {
  RunSimulations();
  RunAnalyses();
}

void Manager::RunSimulations() {
  OpenFilesForOut();
  for(int i =  0; i< simulations_num_; ++i) {
    simulations_.push_back(Simulation(
          1000,
          500,
          500000,
          5000,
          *(out_streams_[i]),
          4,
          Pe_values_[i]));
  }
  std::vector<std::thread> threads;
  for(int i = 0; i<simulations_num_; ++i) {
    threads.push_back(std::thread(
          [this, i](){
            this->simulations_[i].Run();
          }));
  }

  for(auto& thr : threads) {
    thr.join();
  }
  CloseOutFiles();
}

void Manager::RunAnalyses() {
  OpenFilesForIn();
  for(int i = 0; i<simulations_num_; ++i) {
    auto tmp_u_ptr = std::unique_ptr<std::stringstream>(new std::stringstream());
    analyses_results_.push_back(std::move(tmp_u_ptr));
  }
  for(int i = 0 ; i<simulations_num_; ++i) {
    Analyser a(*(in_streams_[i]), *(analyses_results_[i]) );
    a.PerformAnalysis();
  }
  PrintPeValues();
  PrintFetchedData();
  
}

void Manager::PrintPeValues() {
  std::ofstream stream("out/pe_values.out", std::ofstream::out);
  for(auto Pe : Pe_values_){
    stream<<Pe<<" ";
  }
  stream<<'\n';
  stream.close();
}

void Manager::PrintFetchedData() {
  std::ofstream print_stream("out/front_positions.out", std::ofstream::out);
  int step_no;
  int position;
  while(*(analyses_results_[0])>>step_no) {
    std::cout<<"fetching step "<<step_no<<std::endl;
    *(analyses_results_[0])>>position;
    std::cout<<"pos from 0 stream "<<position<<std::endl;
    print_stream<<step_no<<" "<<position<<" ";//print data from first stream
    for(int i = 1; i<simulations_num_; ++i){//print data from other streams
      int step;
      *(analyses_results_[i])>>step;
      if(step!=step_no){
        std::cout<<"Serialization error! "<<step<<" vs "<<step_no<<std::endl;
      }
      *(analyses_results_[i])>>position;
      std::cout<<"pos from " <<i <<" stream "<<position<<std::endl;
      print_stream<<position<<" ";
    }
    print_stream<<'\n';
  }
  print_stream.close();
}

void Manager::GenerateFileNames() {
  std::stringstream converter;
  for(int i = 0; i< simulations_num_; ++i) {
    converter<<"out/sim"<<((i<10)?"00":"0")<<i<<".out\n";
    std::string name;
    converter>>name;
    file_names_.push_back(name);
  }
}

void Manager::OpenFilesForOut() {
  for(auto name : file_names_) {
    std::cout<<"opening file "<<name<<std::endl;
    auto tmp_u_ptr = std::unique_ptr<std::ofstream>(new std::ofstream(name, std::ofstream::out));
    out_streams_.push_back(std::move(tmp_u_ptr));
  }
}

void Manager::OpenFilesForIn() {
  for(auto name : file_names_) {
    std::cout<<"opening file "<<name<<std::endl;
    auto tmp_u_ptr = std::unique_ptr<std::ifstream>(new std::ifstream(name, std::ifstream::in));
    in_streams_.push_back(std::move(tmp_u_ptr));
  }
}

void Manager::CloseOutFiles() {
  for(auto& uptr : out_streams_) {
    uptr->close();
  }
}

void Manager::CloseInFiles() {
  for(auto& uptr : in_streams_) {
    uptr->close();
  }
}

