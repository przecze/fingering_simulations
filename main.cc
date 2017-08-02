#include<manager.h>
#include<analyser.h>
#include<cstring>
#include<iostream>
#include<fstream>

int main(int argc, char** argv){
  //auto manager = Manager({0.3});
  //manager.threads_per_simulation_ = 12;
  //manager.Run();
  std::string filename = "out/sim000";
  std::cout<<"open: "<<filename<<std::endl;
  std::ifstream stream(filename);
  auto analyser = Analyser(stream, std::cout);
  analyser.PerformAnalysis(20000);
  return 0;
}
