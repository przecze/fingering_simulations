#include<manager.h>
#include<cstring>
#include<iostream>

int main(){
  std::cout<<"begin"<<std::endl;
  //try {
    auto manager = Manager({0.04});
    manager.threads_per_simulation_ = 15;
    manager.Run();
  //} catch(...) { std::cout<<"problem!"<<std::endl; }
  return 0;
}
