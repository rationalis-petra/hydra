#include <variant>
#include <iostream>

#include "expressions.hpp"
#include "operations.hpp"

using std::list;
using std::string;


op_if::op_if() { eval_args = false; }
hydra_object* op_if::call(hydra_object* alist, runtime& r) {
  list<hydra_object*> arg_list = get_arg_list(alist, r);
  // aseert that list length is 3
  int len = arg_list.size();
  if (len != 3) {
    throw "arglist to if invalid size!";
  }
  // we now assume that arg_list is a list of length 3

  hydra_object* condition = arg_list.front()->eval(r);
  arg_list.pop_front();
  // is nil?
  if (condition->null()) {
    return arg_list.back()->eval(r);
  }
  // otherwise
  return arg_list.front()->eval(r);
}

op_while::op_while() { eval_args = false; }
hydra_object* op_while::call(hydra_object* alist, runtime& r) {
  list<hydra_object*> arg_list = get_arg_list(alist, r);
  // assert that list length is 3
  int len = arg_list.size();
  if (len < 2) {
    throw "arglist to while invalid size!";
  }
  // we now assume that arg_list is a list of length 3

  hydra_object* condition = arg_list.front();
  arg_list.pop_front();
  // is nil?
  while (!condition->eval(r)->null()) {
    for (hydra_object* o : arg_list) {
      o->eval(r);
    }
  }
  // otherwise
  return new hydra_nil;
}
op_set::op_set() { eval_args = true; }
hydra_object *op_set::call(hydra_object* alist, runtime& r) {
  list<hydra_object*> arg_list = get_arg_list(alist, r);
  if (arg_list.size() != 2) {
    throw "arglist to set invalid size";
  }
  if (hydra_symbol *symbol = dynamic_cast<hydra_symbol*>(arg_list.front())) {
    hydra_object *value = arg_list.back();

    symbol->value = value;
    return value;
  } else {
    string err = "Error: provided non-symbol as first argument of def";
    throw err;
  }
}

op_quote::op_quote() { eval_args = false; }
hydra_object *op_quote::call(hydra_object *alist, runtime &r) {
  list<hydra_object*> arg_list = get_arg_list(alist, r);
  if (arg_list.size() != 1) {
    string err = "invalid number of arguments to quote";
    throw err;
  }
  return arg_list.front();
}

op_eval::op_eval() { eval_args = true; }
hydra_object *op_eval::call(hydra_object* alist, runtime &r) {
  list<hydra_object*> arg_list = get_arg_list(alist, r);
  if (arg_list.size() != 1) {
    string err = "invalid number of arguments to eval";
    throw err;
  }
  return arg_list.front()->eval(r);
}

op_progn::op_progn() { eval_args = false; }
hydra_object *op_progn::call(hydra_object* alist, runtime&r) {
  list<hydra_object*> arg_list = get_arg_list(alist, r);
  hydra_object* out = nullptr;
  for (hydra_object* arg : arg_list) {
    out = arg->eval(r);
  }
  if (!out)
    out = new hydra_nil();
  return out;
}

op_fn::op_fn() { eval_args = false; }
hydra_object *op_fn::call(hydra_object* alist, runtime& r) {
  return new user_oper(alist, true, r);
}

op_mac::op_mac() { eval_args = false; }
hydra_object *op_mac::call(hydra_object* alist, runtime& r) {
  return new user_oper(alist, false, r);
}

op_quit::op_quit() { eval_args = false; }
hydra_object *op_quit::call(hydra_object* alist, runtime& r) {
  exit(0);
}
