#include "expressions.hpp"

using std::string;

void hydra_object_object::mark_node() {
  marked = true;
  for (auto kvp : object_vals) {
    kvp.first->mark_node();
    kvp.second->mark_node();
  }
}

string hydra_object_object::to_string() const {
  string out = "{";
  for (auto x : object_vals) {
    out += "[";
    out += x.first->to_string();
    out += " ";
    out += x.second->to_string();
    out += "]\n";
  }
  out += "}";
  return out;
}
