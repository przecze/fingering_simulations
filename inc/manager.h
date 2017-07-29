#include<vector>
#include<cstring>
#include<fstream>
#include<sstream>
#include<memory>
#include<simulation.h>

class Manager {
 public:
  Manager(
      std::vector<double> Pe_values
      ); 
  void Run();
  void RunSimulations();
  void RunAnalyses();
 private:
  std::vector<std::string> file_names_;
  std::vector<Simulation> simulations_;
  std::vector<std::unique_ptr<std::ofstream>> out_streams_;
  std::vector<std::unique_ptr<std::ifstream>> in_streams_;
  std::vector<double> Pe_values_;
  //std::vector<std::unique_ptr<std::stringstream>> analyses_results_;
  int simulations_num_;
  void GenerateFileNames();
  void CloseInFiles();
  void CloseOutFiles();
  void OpenFilesForOut();
  void OpenFilesForIn();
};


