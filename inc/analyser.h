#ifndef ANALYSER_H
#define ANALYSER_H
#include<iostream>
#include<physical_state.h>

class Analyser {
 public:
  Analyser(
      std::istream& input_stream,
      std::ostream& output_stream
      );
  void PerformAnalysis();
  void RewriteData();
  void SkipFieldFromInput();
  void ReadFieldFromInput();
  void UpdateFrontPosition();
  bool ReadStepLine();
  void PrintAnalysedField(std::ostream&);
  void PrintStepLine(std::ostream&);
 private:
  void AnalyseFingers();
  void AnalyseTips();
  double CalculateTipFlow(int,int,int);
  bool CheckIfLineBurned(int line_num);
  std::istream& input_stream_;
  std::ostream& output_stream_;
  std::unique_ptr<Field> u_;
  std::unique_ptr<Field> v_;
  std::unique_ptr<Field> w_;
  static constexpr double burn_offset_ = 0.9;
  static constexpr double min_v_ = 0.5;
  int current_step_;
  int front_position_;

};

#endif
