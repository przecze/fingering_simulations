#include<manager.h>
#include<analyser.h>
#include<cstring>
#include<iostream>
#include<fstream>

int main(){
  auto manager = Manager({0.05,0.1,0.5,1});
  manager.threads_per_simulation_ = 8;
  manager.Run();
  return 0;
}
