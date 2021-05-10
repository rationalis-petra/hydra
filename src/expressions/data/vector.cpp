
#include <string>
#include <typeinfo>

#include "expressions.hpp"
#include "utils.hpp"

using namespace std;
using namespace expr;
using namespace interp;

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

string Vector::to_string(LocalRuntime &r, LexicalScope &s) {
  string out = "[|";

  for (unsigned i = 0; i < array.size() ; i++) {
    out += hydra_to_string(array[i], r, s);
    if (i != array.size() - 1)
      out += " ";
  }
  out += "|]";
  return out;
}

