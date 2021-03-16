
#include <string>
#include <typeinfo>

#include "expressions.hpp"

using namespace std;
using namespace expr;

Parent* Vector::parent;

int Vector::size() {
  return array.size();
}

Object* Vector::operator[](int i) {
  return array[i];
}

void Vector::mark_node() {
  if (marked) return;
  Object::mark_node();
  for (Object* o : array) {
    o->mark_node();
  }
}

string Vector::to_string() const {
  string out = "(vector ";

  for (unsigned i = 0; i < array.size() ; i++) {
    out += array[i]->to_string();
    if (i != array.size() - 1)
      out += " ";
  }
  out += ")";
  return out;
}

