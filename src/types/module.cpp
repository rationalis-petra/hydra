
#include <string>

#include "expressions.hpp"
#include "types.hpp"

using std::string;
using std::list;

string type_module::to_string() const {
  return "Module";
}

void type_module::mark_node() {
  marked = true;
}

hydra_object *type_module::check_type(hydra_object* obj) {
  if (dynamic_cast<hydra_module*>(obj)) {
    return hydra_t::get();
  } else {
    return hydra_nil::get();
  }
}

hydra_type *type_module::constructor(list<hydra_object*> lst) {
  if (lst.size() == 0) {
    return this;
  } else {
    string err = "Cannot provide argument to the Module Type constructor";
    throw err;
  }
}
