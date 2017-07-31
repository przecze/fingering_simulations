#include<manager.h>
#include<analyser.h>
#include<cstring>
#include<iostream>
#include<fstream>

int main(){
  std::cout<<"begin"<<std::endl;
  //try {
  //} catch(...) { std::cout<<"problem!"<<std::endl; }
  std::ifstream file("sim001.out", std::ifstream::in);
  auto analyser = Analyser(file, std::cout);
  analyser.PerformAnalysis();
  return 0;
}
