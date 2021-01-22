#include "expressions.hpp"

using std::string;

string hydra_symbol::to_string() const {
  return name;
}

hydra_symbol::hydra_symbol(string _name) {
  name = _name;
  value = nullptr;
}

hydra_object* hydra_symbol::eval(runtime& r, lexical_scope& s) {
  // fist, check the lexical scope
  auto loc = s.map.find(this);
  if (loc != s.map.end()) {
    return loc->second; 
  }

  // otherwise, return your value
  if (value == nullptr) {
    string err = "Error: symbol: " + name + " holds no value!";
    throw err;
  } else {
    return value;
  }
}
