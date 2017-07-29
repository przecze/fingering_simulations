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
          100,
          100,
          1000,
          100,
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
  for(auto& in_str : in_streams_) {
    Analyser a(*(in_str), std::cout);
    a.PerformAnalysis();
  }
}

void Manager::GenerateFileNames() {
  std::stringstream converter;
  for(int i = 0; i< simulations_num_; ++i) {
    converter<<"run_outs/sim"<<((i<10)?"00":"0")<<i<<".out\n";
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

