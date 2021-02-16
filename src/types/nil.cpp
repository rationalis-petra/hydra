#include <string>

#include "expressions.hpp"
#include "types.hpp"

using std::string;
using std::list;

void type_nil::mark_node() {
  marked = true;
}

string type_nil::to_string() const {
  return "Nil";
}

hydra_object *type_nil::check_type(hydra_object* obj) {
  return hydra_t::get();
}

hydra_type *type_nil::constructor(list<hydra_object*> lst) {
  if (lst.size() == 0) {
    return this;
  } else {
    string err = "Cannot provide argument to the Nil Type constructor";
    throw err;
  }
}
