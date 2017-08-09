#include<vector>
#include<cstring>
#include<fstream>
#include<sstream>
#include<memory>
#include<simulation.h>
#include<singlemanager.h>

class Manager {
 public:
  Manager(
      std::vector<double> Pe_values
      ); 
  void Run();
  void RunSimulations();
  void RunSingleManagers();
  void RunAnalyses();
  int threads_per_simulation_;
  std::string out_dir_ = "out/";
 private:
  std::vector<std::string> file_names_;
  std::vector<Simulation> simulations_;
  std::vector<std::unique_ptr<SingleManager>> smanagers_;
  std::vector<std::unique_ptr<std::ofstream>> out_streams_;
  std::vector<std::unique_ptr<std::ifstream>> in_streams_;
  std::vector<std::unique_ptr<std::stringstream>> analyses_results_;
  std::vector<double> Pe_values_;
  int simulations_num_;
  void GenerateFileNames();
  void CloseInFiles();
  void CloseOutFiles();
  void OpenFilesForOut();
  void OpenFilesForIn();
  void PrintPeValues();
  void PrintFetchedData();
  void PrintCombinedSMOuts();
};


