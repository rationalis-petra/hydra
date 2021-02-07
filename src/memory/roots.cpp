#include <iostream>
#include "expressions.hpp"

using std::map;

void hydra_roots::remove(hydra_object *obj) {
  map<hydra_object*, unsigned long>::iterator it;
  if ((it = data.find(obj)) != data.end()) {
    data[obj] -= 1;
    if (data[obj] == 0) {
      data.erase(obj);
    }
  }
}

void hydra_roots::insert(hydra_object *obj) {
  map<hydra_object*, unsigned long>::iterator it;
  if ((it = data.find(obj)) != data.end()) {
    data[obj] += 1;
  } else {
    data[obj] = 0;
  }
}

