
#include <string>

#include "expressions.hpp"
#include "types.hpp"

using std::string;

string type_list::to_string() const {
  return "{type list}";
}

hydra_object *type_list::check_type(hydra_object* obj) {
  if ((dynamic_cast<hydra_cons*>(obj)) || obj->null()) {
    return new hydra_t;
  } else {
    return new hydra_nil;
  }
}
