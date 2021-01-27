#include <iostream>
#include <variant>

#include "expressions.hpp"
#include "operations.hpp"

using std::list;
using std::string;

op_if::op_if() { is_fn = false; }
hydra_object *op_if::call(hydra_object *alist, runtime &r, lexical_scope &s) {
  list<hydra_object *> arg_list = get_arg_list(alist, r, s);
  // aseert that list length is 3
  int len = arg_list.size();
  if (len != 3) {
    throw "arglist to if invalid size!";
  }
  // we now assume that arg_list is a list of length 3

  hydra_object *condition = arg_list.front()->eval(r, s);
  arg_list.pop_front();
  // is nil?
  if (condition->null()) {
    return arg_list.back()->eval(r, s);
  }
  // otherwise
  return arg_list.front()->eval(r, s);
}

op_while::op_while() { is_fn = false; }
hydra_object *op_while::call(hydra_object *alist, runtime &r,
                             lexical_scope &s) {
  list<hydra_object *> arg_list = get_arg_list(alist, r, s);
  // assert that list length is 3
  int len = arg_list.size();
  if (len < 2) {
    throw "arglist to while invalid size!";
  }
  // we now assume that arg_list is a list of length 3

  hydra_object *condition = arg_list.front();
  arg_list.pop_front();
  // is nil?
  hydra_object* out = new hydra_nil;
  while (!condition->eval(r, s)->null()) {
    for (hydra_object *o : arg_list) {
      out = o->eval(r, s);
    }
  }
  // otherwise
  return out;
}
op_set::op_set() { is_fn = true; }
hydra_object *op_set::call(hydra_object *alist, runtime &r, lexical_scope &s) {
  list<hydra_object *> arg_list = get_arg_list(alist, r, s);
  if (arg_list.size() != 2) {
    throw "arglist to set invalid size";
  }
  if (hydra_symbol *symbol = dynamic_cast<hydra_symbol *>(arg_list.front())) {
    hydra_object *value = arg_list.back();

    // if there is a lexical scope with the value
    if (s.map.find(symbol) != s.map.end()) {
      s.map[symbol] = value;
    } else if (symbol->mut) {
      symbol->value = value;
    } else {
    string err = "Error: cannot set immutable symbol";
    throw err;
    }

    hydra_object::roots.insert(value);
    hydra_object::collect_garbage(r);
    hydra_object::roots.erase(value);
    return value;
  } else {
    string err = "Error: provided non-symbol as first argument of set";
    throw err;
  }
}

op_quote::op_quote() { is_fn = false; }
hydra_object *op_quote::call(hydra_object *alist, runtime &r,
                             lexical_scope &s) {
  list<hydra_object *> arg_list = get_arg_list(alist, r, s);
  if (arg_list.size() != 1) {
    string err = "invalid number of arguments to quote";
    throw err;
  }
  return arg_list.front();
}

op_eval::op_eval() { is_fn = true; }
hydra_object *op_eval::call(hydra_object *alist, runtime &r, lexical_scope &s) {
  list<hydra_object *> arg_list = get_arg_list(alist, r, s);
  if (arg_list.size() != 1) {
    string err = "invalid number of arguments to eval";
    throw err;
  }
  // eval evaluates in a null lexical scope!
  lexical_scope new_scope;
  hydra_object* value = arg_list.front()->eval(r, new_scope);
  hydra_object::roots.insert(value);
  hydra_object::collect_garbage(r);
  hydra_object::roots.erase(value);
  return value;
}

op_progn::op_progn() { is_fn = false; }
hydra_object *op_progn::call(hydra_object *alist, runtime &r,
                             lexical_scope &s) {
  list<hydra_object *> arg_list = get_arg_list(alist, r, s);
  hydra_object *out = nullptr;
  for (hydra_object *arg : arg_list) {
    out = arg->eval(r, s);
  }
  if (!out)
    out = new hydra_nil();
  return out;
}

op_fn::op_fn() { is_fn = false; }
hydra_object *op_fn::call(hydra_object *alist, runtime &r, lexical_scope &s) {
  return new user_oper(alist, true, r, s);
}

op_mac::op_mac() { is_fn = false; }
hydra_object *op_mac::call(hydra_object *alist, runtime &r, lexical_scope &s) {
  return new user_oper(alist, false, r, s);
}

op_quit::op_quit() { is_fn = false; }
hydra_object *op_quit::call(hydra_object *alist, runtime &r, lexical_scope &s) {
  exit(0);
}

// hydra_object* macroexpand(hydra_object* args) {
// if (hydra_cons* cns = dynamic_cast<hydra_cons*>(args)) {
//   if () {
//   } else if {
//   }
// } else {
//   return args;
// }
//}

// op_macroexpand::op_macroexpand() { is_macro = false; }
// hydra_object *op_macroexpand::call(hydra_object* alist, runtime& r) {
//   macroexpand(alist);
// }
