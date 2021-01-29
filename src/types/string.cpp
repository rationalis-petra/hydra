
#include <string>

#include "expressions.hpp"
#include "types.hpp"

using std::string;

string type_string::to_string() const {
  return "{type string}";
}

hydra_object *type_string::check_type(hydra_object* obj) {
  if (dynamic_cast<hydra_string*>(obj)) {
    return new hydra_t;
  } else {
    return new hydra_nil;
  }
}
