#include <iostream>
#include "expressions.hpp"

using namespace expr;

using std::map;

void hydra_roots::remove(Object *obj) {
  map<Object*, unsigned long>::iterator it;
  if ((it = data.find(obj)) != data.end()) {
    data[obj] -= 1;
    if (data[obj] == 0) {
      data.erase(obj);
    }
  }
}

void hydra_roots::insert(Object *obj) {
  map<Object*, unsigned long>::iterator it;
  if ((it = data.find(obj)) != data.end()) {
    data[obj] += 1;
  } else {
    data[obj] = 0;
  }
}

