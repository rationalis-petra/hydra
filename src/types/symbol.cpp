
#include <string>

#include "expressions.hpp"
#include "types.hpp"

using std::string;

string type_symbol::to_string() const {
  return "{type symbol}";
}

hydra_object *type_symbol::check_type(hydra_object* obj) {
  if (dynamic_cast<hydra_symbol*>(obj)) {
    return new hydra_t;
  } else {
    return new hydra_nil;
  }
}
