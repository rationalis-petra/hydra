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
  type->arg_list.push_front(new type_type);
  type->arg_list.push_front(new type_nil);
}
hydra_object *op_typep::call(hydra_object *alist, runtime &r,
                             lexical_scope &s) {
  list<hydra_object *> arg_list = get_arg_list(alist, r, s);
  if (arg_list.size() != 2) {
    string err = "type? expects exactly 2 arguments";
    throw err;
  }
  hydra_object *obj = arg_list.front();
  hydra_type *type_rep = dynamic_cast<hydra_type*>(arg_list.back());

  return type_rep->check_type(obj);
}

op_type::op_type() {
  is_fn = true;
  docstring = new hydra_string("Calls the constructor for a particular type");
  type->arg_list.push_front(new type_type);
  type->rest_type = new type_nil;
}

hydra_object *op_type::call(hydra_object *alist, runtime &r,
                             lexical_scope &s) {
  list<hydra_object *> arg_list = get_arg_list(alist, r, s);
  hydra_type *type = dynamic_cast<hydra_type*>(arg_list.front());
  arg_list.pop_front();

  return type->constructor(arg_list);
}

op_subtype::op_subtype() {
  is_fn = true;
  docstring = new hydra_string("Returns true if the first argument is a subtype of the second");
  type->arg_list.push_front(new type_type);
  type->arg_list.push_front(new type_type);
}

hydra_object *op_subtype::call(hydra_object *alist, runtime &r,
                             lexical_scope &s) {
  list<hydra_object *> arg_list = get_arg_list(alist, r, s);

  // TODO: implement
  return hydra_t::get();
}
