#include<simulation.h>
#include<cstring>
#include<iostream>
#include<fstream>
int main(){
  std::cout<<"Opening file"<<std::endl; 
  std::ofstream out_file("out.txt", std::ofstream::out); 
  std::cout<<"Creating simulation"<<std::endl; 
  Simulation simulation(100, 100, 100000, 10000, out_file, 4);
  simulation.Run();
  return 0;
}
