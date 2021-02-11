#include <list>
#include <string>

#include "expressions.hpp"
#include "operations.hpp"

using std::list;
using std::string;
using std::to_string;

op_plus::op_plus() {
  is_fn = true;
  docstring = new hydra_string("Returns the sum of its arguemnts. When "
                               "provided with no arguments, returns 0");
  type->rest_type = new type_integer;
}

hydra_object *op_plus::call(hydra_object *alist, runtime &r, lexical_scope &s) {
  // we guarantee that this is a cons object
  std::list<hydra_object *> arg_list = get_arg_list(alist, r, s);
  hydra_num *out = new hydra_num(0);

  for (hydra_object *o : arg_list) {
    hydra_num *num = hydra_cast<hydra_num>(o);
    out->value += num->value;
  }
  return out;
}

op_minus::op_minus() {
  is_fn = true;
  docstring = new hydra_string(
      "Subtract the second and all subsequent arguments from the first");
  type->rest_type = new type_integer;
}

hydra_object *op_minus::call(hydra_object *alist, runtime &r,
                             lexical_scope &s) {
  list<hydra_object *> arg_list = get_arg_list(alist, r, s);
  if (arg_list.size() < 2) {
    throw "Insufficient arguments provided to '-' function";
  }
  hydra_num *out = new hydra_num(0);
  out->value = hydra_cast<hydra_num>(arg_list.front())->value;
  arg_list.pop_front();

  for (hydra_object *arg : arg_list) {
    hydra_num *num = hydra_cast<hydra_num>(arg);
    out->value -= num->value;
  }
  return out;
}

op_multiply::op_multiply() {
  is_fn = true;
  docstring = new hydra_string("Returns the product of all its arguments,\n or "
                               "one if no arguments provided");
  type->rest_type = new type_integer;
}
hydra_object *op_multiply::call(hydra_object *alist, runtime &r,
                                lexical_scope &s) {
  list<hydra_object *> arg_list = get_arg_list(alist, r, s);
  hydra_num *out = new hydra_num(1);

  for (hydra_object *arg : arg_list) {
    if (hydra_num *num = dynamic_cast<hydra_num *>(arg))
      out->value *= num->value;
  }
  return out;
}

op_divide::op_divide() {
  is_fn = true;
  docstring = new hydra_string(
      "Divides the first argument by the second and all subsequent arguments");
  type->rest_type = new type_integer;
}
hydra_object *op_divide::call(hydra_object *alist, runtime &r,
                              lexical_scope &s) {
  list<hydra_object *> arg_list = get_arg_list(alist, r, s);
  if (arg_list.size() < 1) {
    throw "Insufficient arguments provided to '/' function";
  }
  hydra_num *out = new hydra_num(0);

  out->value = dynamic_cast<hydra_num *>(arg_list.front())->value;
  arg_list.pop_front();

  for (hydra_object *arg : arg_list) {
    hydra_num *num = dynamic_cast<hydra_num *>(arg);
    out->value /= num->value;
  }
  return out;
}

op_gr::op_gr() {
  is_fn = true;
  docstring = new hydra_string("Returns t iff the first argument is greater "
                               "than the second, and nil otherwise");
  type->arg_list.push_front(new type_integer);
  type->arg_list.push_front(new type_integer);
}
hydra_object *op_gr::call(hydra_object *alist, runtime &r, lexical_scope &s) {
  list<hydra_object *> arg_list = get_arg_list(alist, r, s);
  // we now ASSERT that arg_list is a list of length 2
  // does it contain integers?

  hydra_num *a1 = dynamic_cast<hydra_num *>(arg_list.front());
  arg_list.pop_front();
  hydra_num *a2 = dynamic_cast<hydra_num *>(arg_list.front());
  // so arg_list is a list containing integers!
  int arg1 = a1->value;
  int arg2 = a2->value;

  if (arg1 >= arg2) {
    return hydra_t::get();
  } else {
    return hydra_nil::get();
  }
}
