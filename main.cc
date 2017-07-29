#include<manager.h>
#include<cstring>
#include<iostream>

int main(){
  std::cout<<"begin"<<std::endl;
  //try {
    auto manager = Manager({10, 11, 12, 13, 14, 15, 16, 17, 18, 19});
    manager.Run();
  //} catch(...) { std::cout<<"problem!"<<std::endl; }
  return 0;
}
