#include <string>

#include "expressions.hpp"
#include "types.hpp"

using std::string;

void type_array::mark_node() {
  marked = true;
}

string type_array::to_string() const {
  return "Vector";
}

hydra_object *type_array::check_type(hydra_object* obj) {
  if ((dynamic_cast<hydra_array*>(obj)) || obj->null()) {
    return hydra_t::get();
  } else {
    return hydra_nil::get();
  }
}
