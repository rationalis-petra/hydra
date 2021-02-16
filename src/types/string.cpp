
#include <string>

#include "expressions.hpp"
#include "types.hpp"

using std::string;
using std::list;

void type_string::mark_node() {
  marked = true;
}

string type_string::to_string() const {
  return "String";
}

hydra_object *type_string::check_type(hydra_object* obj) {
  if (dynamic_cast<hydra_string*>(obj)) {
    return hydra_t::get();
  } else {
    return hydra_nil::get();
  }
}

hydra_type *type_string::constructor(list<hydra_object*> lst) {
  if (lst.size() == 0) {
    return this;
  } else {
    string err = "Cannot provide argument to the String Type constructor";
    throw err;
  }
}
