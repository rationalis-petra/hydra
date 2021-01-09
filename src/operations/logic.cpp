#include <list>

#include "expressions.hpp"
#include "operations.hpp"

using std::list;
using std::string;


op_eq::op_eq() { eval_args = true; }
hydra_object *op_eq::call(hydra_object* alist, runtime& r) {
  list<hydra_object*> arg_list = get_arg_list(alist, r);
  if (arg_list.size() < 2) {
    string err = "Invalid number of arguments to macro =: expects 2";
    throw err;
  }
  hydra_object* arg1 = arg_list.front();
  arg_list.pop_front();
  hydra_object* arg2 = arg_list.front();
  if (hydra_num* num1 = dynamic_cast<hydra_num*>(arg1)) {
    if (hydra_num* num2 = dynamic_cast<hydra_num*>(arg2)) {
      if (num1->value == num2->value)
        return new hydra_t();
      return new hydra_nil();
    }
    return new hydra_nil();
  }
  if (hydra_char* char1 = dynamic_cast<hydra_char*>(arg1)) {
    if (hydra_char* char2 = dynamic_cast<hydra_char*>(arg2)) {
      if (char1->value == char2->value)
        return new hydra_t();
      return new hydra_nil();
    }
    return new hydra_nil();
  }
  if (dynamic_cast<hydra_nil*>(arg1)) {
    if (dynamic_cast<hydra_nil*>(arg2))
      return new hydra_t();
    return new hydra_nil();
  }
  return new hydra_nil();
}

op_or::op_or() { eval_args = false; }
hydra_object *op_or::call(hydra_object* alist, runtime& r) {
  list<hydra_object*> arg_list = get_arg_list(alist, r);
  if (arg_list.size() < 2) {
    string err = "Invalid number of arguments to macro or";
    throw err;
  }

  for (hydra_object* arg : arg_list) {
    hydra_object* cond = arg->eval(r);
    if (!cond->null())
      return cond;
  }
  return new hydra_nil;
}

op_and::op_and() { eval_args = false; }
hydra_object *op_and::call(hydra_object* alist, runtime& r) {
  list<hydra_object*> arg_list = get_arg_list(alist, r);
  if (arg_list.size() < 2) {
    string err = "Invalid number of arguments to macro and";
    throw err;
  }

  hydra_object* out = nullptr;
  for (hydra_object* arg : arg_list) {
    hydra_object* cond = arg->eval(r);
    if (cond->null()) {
      return new hydra_nil();
    }
    else {
      out = cond;
    }
  }
  if (!out)
    out = new hydra_nil();
  return out;
}

op_not::op_not() { eval_args = true; }
hydra_object *op_not::call(hydra_object* alist, runtime& r) {
  list<hydra_object*> arg_list = get_arg_list(alist, r);
  if (arg_list.size() != 1) {
    string err = "Invalid number of arguments to macro not";
    throw err;
  }
  if (alist->null()) {
    return new hydra_t();
  } else {
    return new hydra_nil();
  }
}
