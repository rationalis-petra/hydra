#include <string>

#include "expressions.hpp"
#include "types.hpp"

using std::string;

std::string type_fn::to_string() const {
  return string("{type (") + ") -> " + return_type->to_string() + "}";
}

hydra_object *type_fn::check_type(hydra_object *obj) {
  if (dynamic_cast<hydra_oper*>(obj)) {
    return hydra_t::get();
  }
  return hydra_nil::get();
}


  // hydra_type* return_type;
  // std::list<hydra_type*> arg_list;
  // std::list<hydra_type*> optional_list;
  // std::list<hydra_symbol*> keyword_names;
  // std::list<hydra_type*> keyword_list;
