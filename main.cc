#include<manager.h>
#include<cstring>
#include<iostream>

int main(){
  std::cout<<"begin"<<std::endl;
  auto manager = Manager({0.3});
  manager.Run();
  return 0;
}
