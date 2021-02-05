#include <string>

#include "expressions.hpp"
#include "types.hpp"

using std::string;

string type_nil::to_string() const {
  return "{type Nil}";
}

hydra_object *type_nil::check_type(hydra_object* obj) {
  return hydra_t::get();
}
