
#include <string>
#include <typeinfo>

#include "expressions.hpp"

using namespace std;
using namespace expr;

Parent* Union::parent;

void Union::mark_node() {
  if (marked) return;
  Object::mark_node();
  tag->mark_node();
  value->mark_node();
}

string Union::to_string() const {
  string out = "(Sum ";
  out += tag->to_string();
  out += " ";
  out += value->to_string();
  out += ")";
  return out;
}
