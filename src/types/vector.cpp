#include <string>

#include "expressions.hpp"
#include "types.hpp"

using std::string;

void type_vector::mark_node() {
  marked = true;
}

string type_vector::to_string() const {
  return "Vector";
}

hydra_object *type_vector::check_type(hydra_object* obj) {
  if ((dynamic_cast<hydra_vector*>(obj)) || obj->null()) {
    return hydra_t::get();
  } else {
    return hydra_nil::get();
  }
}
