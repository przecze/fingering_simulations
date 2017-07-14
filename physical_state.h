#include<fstream>
class Field {
 public:
  Field(
      int size_x,
      int size_y
      );
  ~Field();
  double& operator()(int i, int j);
  void Print(std::ostream& stream);
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
  void Step();
  void Print(std::ostream& stream);
  Field u_;
  Field v_;
  Field w_;
 private:
};
