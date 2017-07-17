#include<simulation.h>
#include<cstring>
#include<iostream>
#include<fstream>
int main(){
  std::cout<<"Opening file"<<std::endl; 
  std::ofstream out_file("out.txt", std::ofstream::out); 
  std::cout<<"Creating simulation"<<std::endl; 
  Simulation simulation(300, 300, out_file, 4);
  std::cout<<"Running simulation"<<std::endl; 
  simulation.Run();
  return 0;
}
