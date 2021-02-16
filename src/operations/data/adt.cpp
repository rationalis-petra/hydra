#include <list>

#include "expressions.hpp"
#include "operations.hpp"

using std::list;
using std::string;

op_tuple::op_tuple() {
  is_fn = true;
  docstring =
      new hydra_string("Will return a tuple whose elements are the arg-list");
  type->rest_type = new type_nil;
}

hydra_object *op_tuple::call(hydra_object *alist, runtime &r, lexical_scope &s) {
  list<hydra_object *> arg_list = get_arg_list(alist, r, s);

  hydra_tuple *out = new hydra_tuple;
  for (hydra_object *o : arg_list)
    out->values.push_back(o);
  return out;
}

op_tuple_elt::op_tuple_elt() {
  is_fn = true;
  docstring =
      new hydra_string("Will return a tuple whose elements are the arg-list");
  type->arg_list.push_front(new type_integer);
  type->arg_list.push_front(new type_tuple);
}

hydra_object *op_tuple_elt::call(hydra_object *alist, runtime &r, lexical_scope &s) {
  list<hydra_object *> arg_list = get_arg_list(alist, r, s);

  hydra_tuple *tup = dynamic_cast<hydra_tuple*>(arg_list.front());
  hydra_num *idx = dynamic_cast<hydra_num*>(arg_list.back());

  return tup->values[idx->value];
}


op_union::op_union() {
  is_fn = true;
  docstring =
      new hydra_string("Will return tagged value");
  type->arg_list.push_front(new type_nil);
  type->arg_list.push_front(new type_symbol);
}

hydra_object *op_union::call(hydra_object *alist, runtime &r, lexical_scope &s) {
  list<hydra_object *> arg_list = get_arg_list(alist, r, s);

  hydra_union *out = new hydra_union;
  out->tag = dynamic_cast<hydra_symbol*>(arg_list.front());
  out->value = arg_list.back();
  return out;
}
