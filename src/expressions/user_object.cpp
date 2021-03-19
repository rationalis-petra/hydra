#include "expressions.hpp"
#include "utils.hpp"

using std::string;
using namespace expr;

void UserObject::mark_node() {
  Object::mark_node();
}

string UserObject::to_string(LocalRuntime &r, LexicalScope &s) {
  string out = "\n{";
  if (invoker) 
    out += "invoker " + hydra_to_string(invoker, r, s);
  for (auto x : slots) {
    out += "[";
    out += hydra_to_string(x.first, r, s);
    out += " ";
    out += hydra_to_string(x.second, r, s);

    out += "]";
    if (x != *(--slots.end())) {
      out += "\n";
    }
  }
  out += "}";
  return out;
}

