#include <dlfcn.h>
#include <iostream>
#include "sample.hpp"
using namespace std;

void echo() {
  cout << "お願いダーリン" << endl;
}

void echoC(const char *in) {
  cout << in << endl;
}

void echoI(const int in) {
  cout << in << endl;
}
int main() {
  cout << "HELL WORLD" << endl; 
  echo();
  echoC("hello sample");
  echoI(1234);
}
