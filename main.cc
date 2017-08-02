#include<manager.h>
#include<analyser.h>
#include<cstring>
#include<iostream>
#include<fstream>

void Sim() {
  auto manager = Manager({0.1, 0.3, 0.5});
  manager.threads_per_simulation_ = 12;
  manager.Run();
}

void An() {
  std::string filename = "out/sim000";
  std::cout<<"open: "<<filename<<std::endl;
  std::ifstream stream(filename);
  auto analyser = Analyser(stream, std::cout);
  analyser.PerformAnalysis(20000);
}


int main(int argc, char** argv){
  Sim();
  An();
  return 0;
}
