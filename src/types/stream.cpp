
#include <string>

#include "expressions.hpp"
#include "types.hpp"

using std::string;

string type_stream::to_string() const {
  return "Stream";
}

hydra_object *type_stream::check_type(hydra_object* obj) {
  if ((dynamic_cast<hydra_istream*>(obj)) == nullptr) {
    return hydra_nil::get();
  } else {
    return hydra_t::get();
  }
}
