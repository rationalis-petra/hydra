
#include <string>

#include "expressions.hpp"
#include "types.hpp"

using std::string;
using std::list;

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

hydra_type *type_type::constructor(list<hydra_object*> lst) {
  if (lst.size() == 0) {
    return this;
  } else {
    string err = "Cannot provide argument to the Type Type constructor";
    throw err;
  }
}

