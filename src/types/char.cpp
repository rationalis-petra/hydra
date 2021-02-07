#include <string>

#include "expressions.hpp"
#include "types.hpp"

using std::string;

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
