#include <string>

#include "expressions.hpp"
#include "types.hpp"

using std::string;
using std::list;

void type_char::mark_node() {
  marked = true;
}

string type_char::to_string() const {
  return "Char";
}

hydra_object *type_char::check_type(hydra_object* obj) {
  if ((dynamic_cast<hydra_char*>(obj)) == nullptr) {
    return hydra_nil::get();
  } else {
    return hydra_t::get();
  }
}

hydra_type *type_char::constructor(list<hydra_object*> lst) {
  if (lst.size() == 0) {
    return this;
  } else {
    string err = "Cannot provide argument to the Char Type constructor";
    throw err;
  }
}
