#include <string>

#include "expressions.hpp"
#include "types.hpp"

using std::string;

string type_integer::to_string() const {
  return "integer";
}

hydra_object *type_integer::check_type(hydra_object* obj) {
  if ((dynamic_cast<hydra_num*>(obj)) == nullptr) {
    return new hydra_t;
  } else {
    return new hydra_nil;
  }
}
