#include<simulation.h>
#include<cstring>
#include<iostream>
#include<fstream>
int main(){
  std::ofstream out_file("out.txt", std::ofstream::out); 
  Simulation simulation(5, 5);
  simulation.Run();
  return 0;
}
