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
  void PrintAnalysedField();
  void PerformAnalysis();
  void RewriteData();
 private:
  void SkipFieldFromInput();
  void ReadFieldFromInput();
  void UpdateFrontPosition();
  bool ReadStepLine();
  bool CheckIfLineBurned(int line_num);
  std::istream& input_stream_;
  std::ostream& output_stream_;
  std::unique_ptr<Field> analysed_field_;
  static constexpr double burn_offset_ = 0.9;
  int current_step_;
  int front_position_;

};

#endif
