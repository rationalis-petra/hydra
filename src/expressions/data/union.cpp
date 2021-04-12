
#include <string>
#include <typeinfo>

#include "expressions.hpp"
#include "utils.hpp"

using namespace std;
using namespace expr;
using namespace interp;

Parent* Union::parent;

void Union::mark_node() {
  if (marked) return;
  Object::mark_node();
  tag->mark_node();
  value->mark_node();
}

string Union::to_string(LocalRuntime &r, LexicalScope &s) {
  string out = "(Sum ";
  out += hydra_to_string(tag, r, s);
  out += " ";
  out += hydra_to_string(value, r, s);
  out += ")";
  return out;
}
