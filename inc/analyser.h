#ifndef ANALYSER_H
#define ANALYSER_H
#include<iostream>
#include<physical_state.h>
#include<vector>
class Tip {
 public:
  Tip(int,int);
  void Add(int,int);
  int x_max;
  int x_min;
  int y_max;
  int y_min;
};

class Analyser {
 public:
  Analyser(
      std::istream& input_stream,
      std::ostream& output_stream
      );
  void PerformAnalysis(int start_step=0);
  void RewriteData();
  void SkipToStep(int);
  void LoadFields();
  void SkipFieldFromInput();
  void ReadFieldFromInput();
  void UpdateFrontPosition();
  bool ReadStepLine();
  void FindTips();
  void Mark(Field&, Tip&, int,int);
  void PrintAnalysedField(std::ostream&);
  void PrintStepLine(std::ostream&);
  void PrintTips();
 private:
  bool VPointBurned(int,int);
  void AnalyseFingers();
  void AnalyseTips();
  double CalculateTipLaplace(Tip&);
  double CalculateTipFlow(Tip&);
  bool CheckIfLineBurned(int line_num);
  std::istream& input_stream_;
  std::ostream& output_stream_;
  std::unique_ptr<Field> u_;
  std::unique_ptr<Field> v_;
  std::unique_ptr<Field> w_;
  static constexpr double burn_offset_ = 0.9;
  static constexpr double min_v_ = 0.95;
  std::vector<Tip> tips_;
  int current_step_;
  int front_position_;

};

#endif
