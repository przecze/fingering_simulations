#ifndef ANALYSER_H
#define ANALYSER_H
#include<iostream>
#include<physical_state.h>
#include<vector>
#include<complex>

//struct Vec2D : public std::complex<double> {
// public:
//  inline double& operator[](int n) {
//    if (n==0) return real();
//    else if(n==1) return imag();
//  }
//};

class Tip {
 public:
  Tip(int,int);
  void Add(int,int);
  int Dist(const Tip&, int size_x);
  int x_max;
  int x_min;
  int y_max;
  int y_min;
  int num;
  int x;
  int y;
  double lapl=0;
  double lapl_x=0;
  double lapl_y=0;
  double lapl_ratio=0;
  double flow_ang=0;
  double flow_x=0;
  double flow_y=0;
  double flow=0;
  double vel_x=0;
  double vel_y=0;
  double vel=0;
  double fuel = 0;
  bool has_child=false;
  bool bifurcated=false;
  Tip* parent=nullptr;
  void PrintFlowInfo(std::ostream& output_stream_);
  void PrintLaplInfo(std::ostream& output_stream_);
  void PrintVelInfo(std::ostream& output_stream_);

};

void PrintLaplInfoFormat(std::ostream&);
void PrintFlowInfoFormat(std::ostream&);
void PrintVelInfoFormat(std::ostream&);

class Analyser {
 public:
  Analyser(
      std::istream& input_stream,
      std::ostream& output_stream
      );
  void PerformAnalysis(int start_step=0, int end_step=1e9);
  void RewriteData();
  void SkipToStep(int);
  bool LoadFields();
  void SkipFieldFromInput();
  void ReadFieldFromInput();
  void UpdateFrontPosition();
  bool ReadStepLine();
  void FindTips();
  void Mark(Field&, Tip&, int,int);
  void PrintAnalysedField(std::ostream&);
  void PrintStepLine(std::ostream&);
  void PrintTips();
  void RawPrint();
 private:
  void SortTips();
  bool VPointBurned(int,int);
  void AnalyseFingers();
  void AnalyseTips();
  void PrintAvgLapl();
  void PrintMaxLapl();
  void PrintMinValue();
  void CalculateValuesForTip(Tip&);
  bool CheckIfLineBurned(int line_num);
  std::istream& input_stream_;
  std::ostream& output_stream_;
  std::unique_ptr<Field> u_;
  std::unique_ptr<Field> v_;
  std::unique_ptr<Field> w_;
  static constexpr double burn_offset_ = 0.9;
  static constexpr double min_v_ = 0.5;
  static constexpr int r_for_lapl_calculation_ = 30;
  std::vector<Tip> tips_;
  std::vector<Tip> old_tips_;
  int current_step_;
  int front_position_;
  int tip_num_;

};

#endif
