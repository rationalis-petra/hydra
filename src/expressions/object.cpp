#include "expressions.hpp"

using std::string;
using namespace expr;

void UserObject::mark_node() {
  marked = true;
  for (auto kvp : slots) {
    kvp.first->mark_node();
    kvp.second->mark_node();
  }
}

string UserObject::to_string() const {
  string out = "{";
  for (auto x : slots) {
    out += "[";
    out += x.first->to_string();
    out += " ";
    out += x.second->to_string();
    out += "]\n";
  }
  out += "}";
  return out;
}

