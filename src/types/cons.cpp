#include <string>

#include "expressions.hpp"
#include "types.hpp"

using std::string;

string type_cons::to_string() const {
  return "{type cons}";
}

hydra_object *type_cons::check_type(hydra_object* obj) {
  if ((dynamic_cast<hydra_cons*>(obj)) == nullptr) {
    return new hydra_nil;
  } else {
    return new hydra_t;
  }
}
