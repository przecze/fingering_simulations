#include<simulation.h>
#include<cstring>
#include<iostream>
#include<fstream>
int main(){
  std::ofstream out_file("out.txt", std::ofstream::out); 
  Simulation simulation(100, 100, out_file);
  simulation.Run();
  return 0;
}
