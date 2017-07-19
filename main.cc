#include<simulation.h>
#include<cstring>
#include<iostream>
#include<fstream>
#include<analyser.h>
#include<cstring>

int main(){
  std::cout<<"Opening file"<<std::endl; 
  std::string file_name = "out.txt";
  std::ofstream out_file(file_name, std::ofstream::out); 
  std::cout<<"Creating simulation"<<std::endl; 
  Simulation simulation(200, 100, 100000, 10000, out_file, 4);
  simulation.Run();
  out_file.close();
  std::ifstream in_file(file_name, std::ofstream::in); 
  std::ofstream analyser_out_file("analyser_out.txt", std::ofstream::out); 
  Analyser analyser(in_file, analyser_out_file);
  analyser.RewriteData();
  return 0;
}
