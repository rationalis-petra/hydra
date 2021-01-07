#include <variant>
#include <iostream>

#include "expressions.hpp"
#include "operations.hpp"

using namespace std;



op_if::op_if() { eval_args = false; }
ivy_object* op_if::call(ivy_object* alist, runtime& r) {
  list<ivy_object*> arg_list = get_arg_list(alist, r);
  // aseert that list length is 3
  int len = arg_list.size();
  if (len != 3) {
    throw "arglist to if invalid size!";
  }
  // we now assume that arg_list is a list of length 3

  ivy_object* condition = arg_list.front()->eval(r);
  arg_list.pop_front();
  // is nil?
  if (condition->null()) {
    return arg_list.back()->eval(r);
  }
  // otherwise
  return arg_list.front()->eval(r);
}

op_def::op_def() { eval_args = false; }
ivy_object *op_def::call(ivy_object* alist, runtime& r) {
  list<ivy_object*> arg_list = get_arg_list(alist, r);
  if (arg_list.size() != 2) {
    throw "arglist to def invalid size";
  }
  try {
    ivy_symbol *symbol = dynamic_cast<ivy_symbol*>(arg_list.front());
    arg_list.pop_front();
    ivy_object *value = arg_list.front()->eval(r);

    r.global_store[symbol->symbol] = value;
    return value;
  } catch (bad_cast&) {
    throw "Error: provided non-symbol as first argument of def";
  }
}

op_quote::op_quote() { eval_args = false; }
ivy_object *op_quote::call(ivy_object *alist, runtime &r) {
  list<ivy_object*> arg_list = get_arg_list(alist, r);
  if (arg_list.size() != 1) {
    string err = "invalid number of arguments to quote";
    throw err;
  }
  return arg_list.front();
}

op_eval::op_eval() { eval_args = true; }
ivy_object *op_eval::call(ivy_object* alist, runtime &r) {
  list<ivy_object*> arg_list = get_arg_list(alist, r);
  if (arg_list.size() != 1) {
    string err = "invalid number of arguments to eval";
    throw err;
  }
  return arg_list.front()->eval(r);
}

op_progn::op_progn() { eval_args = false; }
ivy_object *op_progn::call(ivy_object* alist, runtime&r) {
  list<ivy_object*> arg_list = get_arg_list(alist, r);
  ivy_object* out = nullptr;
  for (ivy_object* arg : arg_list) {
    out = arg->eval(r);
  }
  if (!out)
    out = new ivy_nil();
  return out;
}

op_fn::op_fn() { eval_args = false; }
ivy_object *op_fn::call(ivy_object* alist, runtime& r) {
  return new user_oper(alist, true);
}

op_mac::op_mac() { eval_args = false; }
ivy_object *op_mac::call(ivy_object* alist, runtime& r) {
  return new user_oper(alist, false);
}
