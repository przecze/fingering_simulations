#include<simulation.h>
#include<cstring>
#include<iostream>
#include<sstream>
#include<fstream>
#include<analyser.h>
#include<cstring>
void CreateAndRunSimulation(std::ostream& out_file, double Pe){
  std::cout<<"Creating simulation"<<std::endl; 
  int size_x = 200;
  int size_y = 100;
  int steps_total = 30000;
  int big_step = 10000;
  int threads_no = 4;
  Simulation simulation(
      size_x,
      size_y,
      steps_total,
      big_step,
      out_file,
      threads_no);
  simulation.Run();
}

int main(){
  std::stringstream communication_stream;
  std::string sim_out_file_name = "out.txt";
  std::ofstream out_file(sim_out_file_name, std::ofstream::out); 
  CreateAndRunSimulation(out_file, 0);
  out_file.close();

  std::string analyser_in_file_name = sim_out_file_name;
  std::ifstream in_file(analyser_in_file_name, std::ofstream::in); 

  std::ofstream analyser_out_file("analyser_out.txt", std::ofstream::out); 

  Analyser analyser(in_file, analyser_out_file);
  analyser.PerformAnalysis();
  return 0;
}
