#include "expressions.hpp"

using std::string;

string hydra_symbol::to_string() const {
  return name;
}

hydra_symbol::hydra_symbol(string _name) {
  name = _name;
  value = nullptr;
}

hydra_object* hydra_symbol::eval(runtime& r) {
  if (value == nullptr) {
    string err = "Error: symbol: " + name + "holds no value!";
    throw err;
  } else {
    return value;
  }
}
