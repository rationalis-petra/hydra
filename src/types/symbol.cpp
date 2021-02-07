
#include <string>

#include "expressions.hpp"
#include "types.hpp"

using std::string;

void type_symbol::mark_node() {
  marked = true;
}

string type_symbol::to_string() const {
  return "Symbol";
}

hydra_object *type_symbol::check_type(hydra_object* obj) {
  if (dynamic_cast<hydra_symbol*>(obj)) {
    return hydra_t::get();
  } else {
    return hydra_nil::get();
  }
}
