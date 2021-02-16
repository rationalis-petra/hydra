
#include <string>

#include "expressions.hpp"
#include "types.hpp"

using std::string;
using std::list;

void type_list::mark_node() {
  marked = true;
}

string type_list::to_string() const {
  return "{List " + elt_type->to_string() + "}";
}

hydra_object *type_list::check_type(hydra_object* obj) {
  if ((dynamic_cast<hydra_cons*>(obj)) || obj->null()) {
    return hydra_t::get();
  } else {
    return hydra_nil::get();
  }
}

hydra_type *type_list::constructor(list<hydra_object*> lst) {
  type_list* ret = new type_list();

  if (lst.size() == 0) {
    ret->elt_type = new type_nil;
    return ret;
  } else if (lst.size() == 1) {
    if (hydra_type* tp = dynamic_cast<hydra_type*>(lst.front())) {
      ret->elt_type = tp;
      return ret;
    } else {
      string err = "List requires only one argument";
      throw err;
    }
  } else {
    string err = "too many arguments provided to Type List constructor";
    throw err;
  }
}
