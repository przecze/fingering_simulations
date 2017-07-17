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
  double& operator()(int i, int j);
  void Print(std::ostream& stream);
  void Set(double value);
  void Set(std::function<double(int,int)> function);
  void Set(std::function<double(int,int)> function, int x1, int x2, int y1, int y2);
  void Set(double value, int x1, int x2, int y1, int y2);
  void Set(double base, double rand_base, int x1, int x2, int y1, int y2);
  void Set(Field& field);
  void PartialSet(Field& field, int x_begin, int x_end);
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
  void PartialStepCalculation(int x_begin, int x_end);
  void ApplyBoundaryConditions();
  void SwapFieldsWithNew();
  void Print(std::ostream& stream);
  std::unique_ptr<Field> u_;
  std::unique_ptr<Field> v_;
  std::unique_ptr<Field> w_;
  std::unique_ptr<Field> nu_;
  std::unique_ptr<Field> nv_;
  std::unique_ptr<Field> nw_;
 private:
};
