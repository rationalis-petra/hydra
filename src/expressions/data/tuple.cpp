
#include <string>
#include <typeinfo>

#include "expressions.hpp"

using namespace std;
using namespace expr;

Parent* Tuple::parent;

int Tuple::size() {
  return values.size();
}

Object* Tuple::operator[](int i) {
  return values[i];
}

void Tuple::mark_node() {
  if (marked) return;
  Object::mark_node();
  for (Object* o : values) {
    o->mark_node();
  }
}

string Tuple::to_string() const {
  string out = "[";

  for (unsigned i = 0; i < values.size() ; i++) {
    out += values[i]->to_string();
    if (i != values.size() - 1)
      out += " ";
  }
  out += "]";
  return out;
}
