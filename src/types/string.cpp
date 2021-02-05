
#include <string>

#include "expressions.hpp"
#include "types.hpp"

using std::string;

string type_string::to_string() const {
  return "{type String}";
}

hydra_object *type_string::check_type(hydra_object* obj) {
  if (dynamic_cast<hydra_string*>(obj)) {
    return hydra_t::get();
  } else {
    return hydra_nil::get();
  }
}
