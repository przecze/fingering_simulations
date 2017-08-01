#include<manager.h>
#include<analyser.h>
#include<cstring>
#include<iostream>
#include<fstream>

int main(int argc, char** argv){
  //auto manager = Manager({0.05,0.1,0.5,1});
  //manager.threads_per_simulation_ = 8;
  //manager.Run();
  std::string filename = argv[1];
  std::cout<<"open: "<<filename<<std::endl;
  std::ifstream stream(filename);
  auto analyser = Analyser(stream, std::cout);
  analyser.PerformAnalysis(20000);
  return 0;
}
