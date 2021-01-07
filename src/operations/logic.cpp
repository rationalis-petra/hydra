#include <list>

#include "expressions.hpp"
#include "operations.hpp"

using namespace std;



op_eq::op_eq() { eval_args = true; }
ivy_object *op_eq::call(ivy_object* alist, runtime& r) {
  list<ivy_object*> arg_list = get_arg_list(alist, r);
  if (arg_list.size() < 2) {
    throw "Invalid number of arguments to macro =: expects 2";
  }
  ivy_object* arg1 = arg_list.front();
  arg_list.pop_front();
  ivy_object* arg2 = arg_list.front();
  if (ivy_num* num1 = dynamic_cast<ivy_num*>(arg1)) {
    if (ivy_num* num2 = dynamic_cast<ivy_num*>(arg2)) {
      if (num1->val == num2->val)
        return new ivy_t();
      return new ivy_nil();
    }
    return new ivy_nil();
  }
  if (dynamic_cast<ivy_nil*>(arg1)) {
    if (dynamic_cast<ivy_nil*>(arg2))
      return new ivy_t();
    return new ivy_nil();
  }
  return new ivy_nil();
}

op_or::op_or() { eval_args = false; }
ivy_object *op_or::call(ivy_object* alist, runtime& r) {
  list<ivy_object*> arg_list = get_arg_list(alist, r);
  if (arg_list.size() < 2) {
    throw "Invalid number of arguments to macro or";
  }

  for (ivy_object* arg : arg_list) {
    ivy_object* cond = arg->eval(r);
    if (!cond->null())
      return cond;
  }
  return new ivy_nil;
}

op_and::op_and() { eval_args = false; }
ivy_object *op_and::call(ivy_object* alist, runtime& r) {
  list<ivy_object*> arg_list = get_arg_list(alist, r);
  if (arg_list.size() < 2) {
    throw "Invalid number of arguments to macro and";
  }

  ivy_object* out = nullptr;
  for (ivy_object* arg : arg_list) {
    ivy_object* cond = arg->eval(r);
    if (cond->null()) {
      return new ivy_nil();
    }
    else {
      out = cond;
    }
  }
  if (!out)
    out = new ivy_nil();
  return out;
}

op_not::op_not() { eval_args = true; }
ivy_object *op_not::call(ivy_object* alist, runtime& r) {
  list<ivy_object*> arg_list = get_arg_list(alist, r);
  if (arg_list.size() != 1) {
    throw "Invalid number of arguments to macro not";
  }
  if (alist->null()) {
    return new ivy_t();
  } else {
    return new ivy_nil();
  }
}
