
#include <string>

#include "expressions.hpp"
#include "types.hpp"

using std::string;

void type_type::mark_node() {
  marked = true;
}

string type_type::to_string() const {
  return "Type";
}

hydra_object *type_type::check_type(hydra_object* obj) {
  if (dynamic_cast<hydra_type*>(obj)) {
    return hydra_t::get();
  } else {
    return hydra_nil::get();
  }
}
