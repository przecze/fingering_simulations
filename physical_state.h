#include<fstream>
#include<functional>
#include<memory>

class Field {
 public:
  Field(
      int size_x,
      int size_y
      );
  ~Field();
  inline double& operator()(int i, int j){
    if(i>=0 && i<size_x_ && j>=0 && j<size_y_)
      return data_[i][j];
    else
      return data_[(i+size_x_)%size_x_][(j+size_y_)%size_y_];
  }
  void Print(std::ostream& stream);
  void SetValue(double value);
  void SetValuePart(double value, int x1, int x2, int y1, int y2);
  void SetRandomPart(double base, double multiplier, int x1, int x2, int y1, int y2);
  int size_x_;
  int size_y_;
 private:
  double** data_;
};

class PhysicalState {
 public:
  PhysicalState(
      int size_x,
      int size_y
      );
  int size_x_;
  int size_y_;
  void Print(std::ostream& stream);
  Field u_;
  Field v_;
  Field w_;
 private:
};
