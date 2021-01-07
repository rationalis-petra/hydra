#include <list>

#include "expressions.hpp"
#include "operations.hpp"

using namespace std;

op_cons::op_cons() { eval_args = true; }
hydra_object *op_cons::call(hydra_object *alist, runtime &r) {
  list<hydra_object *> arg_list = get_arg_list(alist, r);
  if (arg_list.size() != 2) {
    throw "invalid number of args to cons";
  }

  hydra_object *car = arg_list.front();
  arg_list.pop_front();
  hydra_object *cdr = arg_list.front();

  if (!(dynamic_cast<hydra_cons*>(cdr) || dynamic_cast<hydra_nil*>(cdr))) {
    string err = "Non-list provided as second argument to cons";
    throw err;
  }

  hydra_cons* out = new hydra_cons();
  out->car = car;
  out->cdr = cdr;
  return out;
}

op_cdr::op_cdr() { eval_args = true; }
hydra_object *op_cdr::call (hydra_object* alist, runtime& r) {
  list<hydra_object*> arg_list = get_arg_list(alist, r);
  if (arg_list.size() != 1) {
    throw "invalid number of args to cdr";
  }

  try {
    hydra_object *arg = arg_list.front();
    hydra_cons* cons = dynamic_cast<hydra_cons*>(arg);
    return cons->cdr;
  } catch (bad_cast&) {
    string err = "Non-cons argument provided to cdr: " + arg_list.front()->to_string();
    throw err;
  }
}

op_car::op_car() { eval_args = true; }
hydra_object *op_car::call (hydra_object* alist, runtime& r) {
  list<hydra_object*> arg_list = get_arg_list(alist, r);
  if (arg_list.size() != 1) {
    throw "invalid number of args to car";
  }

  try {
    hydra_object *arg = arg_list.front();
    hydra_cons* cons = dynamic_cast<hydra_cons*>(arg);
    return cons->car;
  } catch (bad_cast&) {
    string err = "Non-cons argument provided to car: " + arg_list.front()->to_string();
    throw err;
  }
}

op_arr::op_arr() { eval_args = true; }
hydra_object *op_arr::call(hydra_object *alist, runtime &r) {
  list<hydra_object*> arg_list = get_arg_list(alist, r);

  hydra_array* out = new hydra_array;
  for (hydra_object* o : arg_list)
    out->array.push_back(o);
  return out;
}
