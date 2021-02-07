#include <string>

#include "expressions.hpp"
#include "types.hpp"

using std::string;

void type_integer::mark_node() {
  marked = true;
}

string type_integer::to_string() const {
  return "Integer";
}

hydra_object *type_integer::check_type(hydra_object* obj) {
  if (dynamic_cast<hydra_num*>(obj)) {
    return hydra_t::get();
  } else {
    return hydra_nil::get();
  }
}
