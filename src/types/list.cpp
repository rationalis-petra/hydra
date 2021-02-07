
#include <string>

#include "expressions.hpp"
#include "types.hpp"

using std::string;

void type_list::mark_node() {
  marked = true;
}

string type_list::to_string() const {
  return "List";
}

hydra_object *type_list::check_type(hydra_object* obj) {
  if ((dynamic_cast<hydra_cons*>(obj)) || obj->null()) {
    return hydra_t::get();
  } else {
    return hydra_nil::get();
  }
}
