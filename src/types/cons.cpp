#include <string>

#include "expressions.hpp"
#include "types.hpp"

using std::string;

void type_cons::mark_node() {
  marked = true;
}

string type_cons::to_string() const {
  return "Cons";
}

hydra_object *type_cons::check_type(hydra_object* obj) {
  if ((dynamic_cast<hydra_cons*>(obj)) == nullptr) {
    return hydra_nil::get();
  } else {
    return hydra_t::get();
  }
}
