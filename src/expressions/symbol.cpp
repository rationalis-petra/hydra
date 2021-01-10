#include "expressions.hpp"

using std::string;

string hydra_symbol::to_string() const {
  return symbol;
}

hydra_object* hydra_symbol::eval(runtime& r) {
  if (r.global_store.find(symbol) != r.global_store.end()) {
    return r.global_store[symbol];
  }
  else {
    string err = "error: symbol: " + symbol + " cannot be found";
    throw err;
  }
}
