#include <string>

#include "expressions.hpp"

using std::string;
using std::map;

hydra_module::hydra_module(string _name) {
  name = _name;
}

string hydra_module::to_string() const {
  return "<module>";
}

hydra_symbol *hydra_module::intern(string str) {
  hydra_object* out = get(str);
  if (out->null()) {
    hydra_symbol *sym = new hydra_symbol(str);
    symbols[str] = sym;
    return sym;
  } else {
    return static_cast<hydra_symbol*>(out);
  }
}

hydra_object *hydra_module::get(string str) {
  map<string, hydra_symbol*>::iterator loc = symbols.find(str);
  if (loc == symbols.end()) {
    return new hydra_nil;
  }
  return loc->second;
}

