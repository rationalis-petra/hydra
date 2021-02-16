
#include <string>

#include "expressions.hpp"
#include "types.hpp"

using std::string;
using std::list;

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

hydra_type *type_symbol::constructor(list<hydra_object*> lst) {
  if (lst.size() == 0) {
    return this;
  } else {
    string err = "Cannot provide argument to the Symbol Type constructor";
    throw err;
  }
}
