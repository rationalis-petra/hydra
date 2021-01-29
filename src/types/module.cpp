
#include <string>

#include "expressions.hpp"
#include "types.hpp"

using std::string;

string type_module::to_string() const {
  return "{type module}";
}

hydra_object *type_module::check_type(hydra_object* obj) {
  if (dynamic_cast<hydra_module*>(obj)) {
    return new hydra_t;
  } else {
    return new hydra_nil;
  }
}
