#include <list>

#include "expressions.hpp"
#include "operations.hpp"

using namespace std;

op_cons::op_cons() { eval_args = true; }
ivy_object *op_cons::call(ivy_object *alist, runtime &r) {
  list<ivy_object *> arg_list = get_arg_list(alist, r);
  if (arg_list.size() != 2) {
    throw "invalid number of args to cons";
  }

  ivy_object *car = arg_list.front();
  arg_list.pop_front();
  ivy_object *cdr = arg_list.front();

  if (!(dynamic_cast<ivy_cons*>(cdr) || dynamic_cast<ivy_nil*>(cdr))) {
    string err = "Non-list provided as second argument to cons";
    throw err;
  }

  ivy_cons* out = new ivy_cons();
  out->car = car;
  out->cdr = cdr;
  return out;
}

op_cdr::op_cdr() { eval_args = true; }
ivy_object *op_cdr::call (ivy_object* alist, runtime& r) {
  list<ivy_object*> arg_list = get_arg_list(alist, r);
  if (arg_list.size() != 1) {
    throw "invalid number of args to cdr";
  }

  try {
    ivy_object *arg = arg_list.front();
    ivy_cons* cons = dynamic_cast<ivy_cons*>(arg);
    return cons->cdr;
  } catch (bad_cast&) {
    string err = "Non-cons argument provided to cdr: " + arg_list.front()->to_string();
    throw err;
  }
}

op_car::op_car() { eval_args = true; }
ivy_object *op_car::call (ivy_object* alist, runtime& r) {
  list<ivy_object*> arg_list = get_arg_list(alist, r);
  if (arg_list.size() != 1) {
    throw "invalid number of args to car";
  }

  try {
    ivy_object *arg = arg_list.front();
    ivy_cons* cons = dynamic_cast<ivy_cons*>(arg);
    return cons->car;
  } catch (bad_cast&) {
    string err = "Non-cons argument provided to car: " + arg_list.front()->to_string();
    throw err;
  }
}

op_arr::op_arr() { eval_args = true; }
ivy_object *op_arr::call(ivy_object *alist, runtime &r) {
  list<ivy_object*> arg_list = get_arg_list(alist, r);

  ivy_array* out = new ivy_array;
  for (ivy_object* o : arg_list)
    out->array.push_back(o);
  return out;
}
