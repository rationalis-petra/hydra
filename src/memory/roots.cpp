#include <iostream>
#include "expressions.hpp"

using namespace expr;

using std::map;

void hydra_roots::remove(Value *obj) {
  map<Value*, unsigned long>::iterator it;
  if ((it = data.find(obj)) != data.end()) {
    data[obj] -= 1;
    if (data[obj] == 0) {
      data.erase(obj);
    }
  }
}

void hydra_roots::insert(Value *obj) {
  map<Value*, unsigned long>::iterator it;
  if ((it = data.find(obj)) != data.end()) {
    data[obj] += 1;
  } else {
    data[obj] = 0;
  }
}

