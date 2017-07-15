#include<fstream>
#include<functional>
class Field {
 public:
  Field(
      int size_x,
      int size_y
      );
  ~Field();
  double& operator()(int i, int j);
  void Print(std::ostream& stream);
  void Set(double value);
  void Set(std::function<double(int,int)> function);
  void Set(std::function<double(int,int)> function, int x1, int x2, int y1, int y2);
  void Set(double value, int x1, int x2, int y1, int y2);
  void Set(double base, double rand_base, int x1, int x2, int y1, int y2);
  void Set(Field& field);
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
  void InitValues();
  void Step();
  void JapanStep();
  void GrayScottStep();
  void Print(std::ostream& stream);
  Field u_;
  Field v_;
  Field w_;
 private:
};
