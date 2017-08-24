#include<manager.h>
#include<singlemanager.h>
#include<analyser.h>
#include<cstring>
#include<iostream>
#include<fstream>
#include<ctime>
#include<cstdlib>

void SingleManagerTest() {

  std::string out_filename = "sm_out";
  std::cout<<"open: "<<out_filename<<std::endl;
  std::ofstream data_out(out_filename);

  out_filename = "analyser_out";
  std::cout<<"open: "<<out_filename<<std::endl;
  std::ofstream analyser_out(out_filename);

  SingleManager simple(data_out, analyser_out, 0.3);
  simple.Init();
  simple.Run();
  
}

void Sim() {
  auto manager = Manager({0.01, 0.02, 0.05, 0.1, 0.2,0.5,1,2,5,10,20});
  std::string out_dir = "runs/run019/";
  std::system((std::string("mkdir -p ") + out_dir).c_str());
  manager.out_dir_ = out_dir;
  manager.threads_per_simulation_ = 8;
  manager.Run();
}

void An() {
  std::string filename = "runs/run005/sim001.out";
  std::cout<<"open: "<<filename<<" for input"<<std::endl;
  std::ifstream stream(filename);
  std::string out_filename = "analyser_out";
  std::cout<<"open: "<<out_filename<<std::endl;
  std::ofstream out_stream(out_filename);
  auto analyser = Analyser(stream, out_stream);
  analyser.PerformAnalysis(20000, 150000);
}


int main(int argc, char** argv){
  std::srand(time(nullptr));
  Sim();
  return 0;
}
