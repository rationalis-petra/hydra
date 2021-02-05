#include <string>

#include "expressions.hpp"
#include "types.hpp"

using std::string;

string type_cons::to_string() const {
  return "{type Cons}";
}

hydra_object *type_cons::check_type(hydra_object* obj) {
  if ((dynamic_cast<hydra_cons*>(obj)) == nullptr) {
    return hydra_nil::get();
  } else {
    return hydra_t::get();
  }
}
