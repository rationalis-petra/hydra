#include <iostream>
#include "expressions.hpp"

using namespace expr;
using namespace interp;

void GarbageRoots::remove(Object *obj) {
  auto it = data.find(obj);
  if (it != data.end()) {
    data[obj] -= 1;
    if (data[obj] == 0) {
      data.erase(obj);
    }
  }
}

void GarbageRoots::insert(Object *obj) {
  auto it = data.find(obj);
  if (it != data.end()) {
    data[obj] += 1;
  } else {
    data[obj] = 0;
  }
}

