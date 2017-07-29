#include<manager.h>
#include<cstring>
#include<iostream>

int main(){
  std::cout<<"begin"<<std::endl;
  //try {
    auto manager = Manager({0.5, 1., 1.5, 2., 3., 4., 5., 6., 8., 10.});
    manager.Run();
  //} catch(...) { std::cout<<"problem!"<<std::endl; }
  return 0;
}
