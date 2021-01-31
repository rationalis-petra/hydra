#include <list>
#include <string>

#include "expressions.hpp"
#include "operations/types.hpp"
#include "types.hpp"

using std::list;
using std::string;

op_typep::op_typep() {
  is_fn = true;
  docstring = new hydra_string("Returns t if the first argument is the type defined by the\n"
                               "second, otherwise returns nil");
}
hydra_object *op_typep::call(hydra_object *alist, runtime &r,
                             lexical_scope &s) {
  list<hydra_object *> arg_list = get_arg_list(alist, r, s);
  if (arg_list.size() != 2) {
    string err = "typep expects exactly 2 arguments";
    throw err;
  }
  hydra_object *obj = arg_list.front();
  hydra_object *type_rep = arg_list.back();

  hydra_type *type;
  if (!(type = dynamic_cast<hydra_type*>(type_rep))) {
    type = type_from_rep(type_rep);
  }
  return type->check_type(obj);
}

op_type::op_type() {
  is_fn = true;
  docstring = new hydra_string("Generates a type from the provided symbol argument");
}
hydra_object *op_type::call(hydra_object *alist, runtime &r,
                            lexical_scope &s) {
  list<hydra_object*> arg_list = get_arg_list(alist, r, s);
  if (arg_list.size() != 1) {
    string err = "type expects exactly 1 argument";
    throw err;
  }
  return type_from_rep(arg_list.front());
}
