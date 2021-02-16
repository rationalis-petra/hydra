#include <string>
#include <list>

#include "expressions.hpp"
#include "types.hpp"

using std::string;
using std::list;

void type_cons::mark_node() {
  marked = true;
}

string type_cons::to_string() const {
  return "Cons";
}

hydra_object *type_cons::check_type(hydra_object* obj) {
  if ((dynamic_cast<hydra_cons*>(obj)) == nullptr) {
    return hydra_nil::get();
  } else {
    return hydra_t::get();
  }
}

hydra_type *type_cons::constructor(list<hydra_object*> lst) {
  if (lst.size() == 0) {
    return this;
  } else {
    string err = "Cannot provide argument to the Char Type constructor";
    throw err;
  }
}
