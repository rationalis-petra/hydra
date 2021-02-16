#include <string>

#include "expressions.hpp"
#include "types.hpp"

using std::string;
using std::list;

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

hydra_type *type_vector::constructor(list<hydra_object*> lst) {
  if (lst.size() == 0) {
    return this;
  } else {
    string err = "unimplemented...";
    throw err;
  }
}
