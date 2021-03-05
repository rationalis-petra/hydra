#include <string>
#include <list>

#include "expressions.hpp"
#include "types.hpp"

using std::string;
using std::list;
using std::set;

void type_object::mark_node() {
  marked = true;
}

string type_object::to_string() const {
  return "Obejct";
}

hydra_object *type_object::check_type(hydra_object* obj) {
  if ((dynamic_cast<hydra_object_object *>(obj)) == nullptr) {
    return hydra_nil::get();
  } else {
    return hydra_t::get();
  }
}

hydra_type *type_object::constructor(list<hydra_object*> lst) {
  return this;
}
