#include<manager.h>

#include<iostream>
#include<sstream>
#include<iomanip>
#include<fstream>
#include<thread>

#include<simulation.h>
#include<analyser.h>
#include<singlemanager.h>


Manager::Manager(std::vector<double> Pe_values) :
    Pe_values_(Pe_values),
    simulations_num_(Pe_values.size()),
    threads_per_simulation_(4) {
  std::cout<<"manager created"<<std::endl;
}

void Manager::Run() {
  RunSingleManagers();
  //RunSimulations();
  //RunAnalyses();
}

void Manager::RunSimulations() {
  GenerateFileNames();
  PrintPeValues();
  OpenFilesForOut();
  for(int i =  0; i< simulations_num_; ++i) {
    simulations_.push_back(Simulation(
          800,
          800,
          1200000,
          50000,
          *(out_streams_[i]),
          threads_per_simulation_,
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

void Manager::RunSingleManagers() {
  GenerateFileNames();
  OpenFilesForOut();
  for(int i = 0; i<simulations_num_; ++i) {
    auto tmp_u_ptr = std::unique_ptr<std::stringstream>(new std::stringstream());
    analyses_results_.push_back(std::move(tmp_u_ptr));
  }
  for(int i =  0; i< simulations_num_; ++i) {
    auto tmp_u_ptr = std::unique_ptr<SingleManager>(new SingleManager(
          *(out_streams_[i]),
          *(analyses_results_[i]),
          Pe_values_[i]
          ));
    smanagers_.push_back(std::move(tmp_u_ptr));
  }
  std::vector<std::thread> threads;
  for(int i = 0; i<simulations_num_; ++i) {
    smanagers_[i]->Init();
    threads.push_back(std::thread(
          [this, i](){
            this->smanagers_[i]->Run();
          }));
  }

  for(auto& thr : threads) {
  std::cout<<"streams created"<<std::endl;
    thr.join();
  }
  CloseOutFiles();
  PrintCombinedSMOuts();
}
void Manager::RunAnalyses() {
  OpenFilesForIn();
  for(int i = 0; i<simulations_num_; ++i) {
    auto tmp_u_ptr = std::unique_ptr<std::stringstream>(new std::stringstream());
    analyses_results_.push_back(std::move(tmp_u_ptr));
  }
  for(int i = 0 ; i<simulations_num_; ++i) {
    Analyser a(*(in_streams_[i]), std::cout);
    a.PerformAnalysis();
  }
  PrintFetchedData();
  
}

void Manager::PrintPeValues() {
  std::ofstream stream(out_dir_+"pe_values.out", std::ofstream::out);
  for(auto Pe : Pe_values_){
    stream<<Pe<<" ";
  }
  stream<<'\n';
  stream.close();
}

void Manager::PrintFetchedData() {
  std::ofstream print_stream(out_dir_+"front_positions.out", std::ofstream::out);
  int step_no;
  int position;
  print_stream<<"Step\n";
  for(auto Pe : Pe_values_){
    print_stream<<Pe<<" ";
  }
  print_stream<<'\n';
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

void Manager::PrintCombinedSMOuts() {
  std::ofstream print_stream(out_dir_+"pe_vs_properties.out", std::ofstream::out);
  for(int i = 0; i<simulations_num_; ++i){
    std::string data(std::istreambuf_iterator<char>(*analyses_results_[i]), {});
    //(*analyses_results_[i])>>data;
    std::cout<<data<<std::endl;
    print_stream<<Pe_values_[i]<<" "<<data;
  }
  print_stream.close();
}

void Manager::GenerateFileNames() {
  std::stringstream converter;
  for(int i = 0; i< simulations_num_; ++i) {
    converter<<out_dir_+"sim"<<((i<10)?"00":"0")<<i<<"\n";
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

