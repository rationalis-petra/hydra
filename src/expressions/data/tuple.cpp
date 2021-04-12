
#include <string>
#include <typeinfo>

#include "utils.hpp"
#include "expressions.hpp"

using namespace std;
using namespace expr;
using namespace interp;

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

string Tuple::to_string(LocalRuntime &r, LexicalScope &s) {
  string out = "[";

  for (unsigned i = 0; i < values.size() ; i++) {
    out += hydra_to_string(values[i], r, s);
    if (i != values.size() - 1)
      out += " ";
  }
  out += "]";
  return out;
}
