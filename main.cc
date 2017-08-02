#include<manager.h>
#include<analyser.h>
#include<cstring>
#include<iostream>
#include<fstream>
#include<ctime>
#include<cstdlib>

void Sim() {
  auto manager = Manager({0.1, 0.2, 0.3, 0.5});
  manager.threads_per_simulation_ = 8;
  manager.Run();
}

void An() {
  std::string filename = "out/sim002";
  std::cout<<"open: "<<filename<<std::endl;
  std::ifstream stream(filename);
  auto analyser = Analyser(stream, std::cout);
  analyser.PerformAnalysis(20000);
}


int main(int argc, char** argv){
  std::srand(time(nullptr));
  Sim();
  An();
  return 0;
}
