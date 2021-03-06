#include <iostream>
#include <variant>

#include "expressions.hpp"
#include "operations.hpp"

using std::list;
using std::string;

op_if::op_if() {
  is_fn = false;
  type->arg_list.push_front(new type_nil);
  type->arg_list.push_front(new type_nil);
  type->arg_list.push_front(new type_nil);
}

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

op_while::op_while() { is_fn = false;
  type->rest_type = new type_nil;
  }
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
  hydra_object* out = hydra_nil::get();
  while (!condition->eval(r, s)->null()) {
    for (hydra_object *o : arg_list) {
      out = o->eval(r, s);
    }
  }
  // otherwise
  return out;
}
op_set::op_set() {
  is_fn = true;

  docstring = new hydra_string("Sets the value symbol (first argument) to the second argument");

  type->arg_list.push_front(new type_nil);
  // OR symbol ref
  type->arg_list.push_front(new type_symbol);
}
hydra_object *op_set::call(hydra_object *alist, runtime &r, lexical_scope &s) {
  list<hydra_object *> arg_list = get_arg_list(alist, r, s);

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

    hydra_object::roots.insert(symbol);
    hydra_object::collect_garbage(r);
    hydra_object::roots.remove(symbol);
    return value;
  } else {
    string err = "Error: provided non-symbol as first argument of set";
    throw err;
  }
}

op_unset::op_unset() {
  is_fn = true;

  docstring = new hydra_string("Removes the value from a symbol");

  type->arg_list.push_front(new type_symbol);
}
hydra_object *op_unset::call(hydra_object *alist, runtime &r, lexical_scope &s) {
  list<hydra_object *> arg_list = get_arg_list(alist, r, s);

  hydra_symbol *symbol = dynamic_cast<hydra_symbol *>(arg_list.front());

  // if there is a lexical scope with the value
  if (s.map.find(symbol) != s.map.end()) {
    //s.map[symbol] = nullptr;
    s.map.erase(symbol);
  } else if (symbol->mut) {
    symbol->value = nullptr;
  } else {
    string err = "Error: cannot set immutable symbol";
    throw err;
  }

  hydra_object::roots.insert(symbol);
  hydra_object::collect_garbage(r);
  hydra_object::roots.remove(symbol);
  return symbol;
}

op_defined::op_defined() {
  is_fn = true;
  docstring = new hydra_string("Returns t if symbol contains a value, and nil otherwise");
  type->arg_list.push_back(new type_symbol);
};

hydra_object* op_defined::call(hydra_object *alist, runtime &r, lexical_scope &s) {
  list<hydra_object *> arg_list = get_arg_list(alist, r, s);
  hydra_symbol* sym = hydra_cast<hydra_symbol>(arg_list.front());
  if (sym->value) {
    return hydra_t::get();
  } else {
    return hydra_nil::get();
  }
}

op_quote::op_quote() {
  is_fn = false;
  docstring = new hydra_string("Prevents evaluation of the argument it is provided");
  type->arg_list.push_front(new type_nil);
}
hydra_object *op_quote::call(hydra_object *alist, runtime &r,
                             lexical_scope &s) {
  list<hydra_object *> arg_list = get_arg_list(alist, r, s);
  return arg_list.front();
}

op_eval::op_eval() {
  is_fn = true;
  docstring = new hydra_string("Evaluates it's argument as if it was a hydra program");
  type->arg_list.push_front(new type_nil);
}
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
  hydra_object::roots.remove(value);
  return value;
}

op_progn::op_progn() {
  is_fn = false;
  docstring = new hydra_string("Evaluates its arguments in sequential order, returning the value\n"
                               "of the last one");
  type->rest_type = new type_nil;
}
hydra_object *op_progn::call(hydra_object *alist, runtime &r,
                             lexical_scope &s) {
  list<hydra_object *> arg_list = get_arg_list(alist, r, s);
  hydra_object *out = nullptr;
  for (hydra_object *arg : arg_list) {
    out = arg->eval(r, s);
  }
  if (!out)
    out = hydra_nil::get();
  return out;
}

op_gen::op_gen() {
  docstring = new hydra_string("Generates a new generic function object");
  is_fn = false;
  type->arg_list.push_front(new type_list);
  type->rest_type = new type_nil;
}
hydra_object *op_gen::call(hydra_object *alist, runtime &r, lexical_scope &s) {
  list<hydra_object*> arg_list = get_arg_list(alist, r, s);
  combined_fn* out = new combined_fn;
  out->type->rest_type = new type_nil;
  arg_list.pop_front();
  if (!arg_list.empty()) {
    if (hydra_string* str = dynamic_cast<hydra_string*>(arg_list.front())) {
      out->docstring = str;
    }
  }
  return out;
}

op_fn::op_fn() {
  is_fn = false;
  docstring = new hydra_string("Generates a new function object");
  type->rest_type = new type_nil;
}
hydra_object *op_fn::call(hydra_object *alist, runtime &r, lexical_scope &s) {
  return (hydra_object*) new user_oper(alist, true, r, s);
}

op_mac::op_mac() {
  is_fn = false;
  docstring = new hydra_string("Generates a new macro object");
  type->arg_list.push_front(new type_cons);
  type->rest_type = new type_nil;
}
hydra_object *op_mac::call(hydra_object *alist, runtime &r, lexical_scope &s) {
  return (hydra_object*) new user_oper(alist, false, r, s);
}

op_addfn::op_addfn() {
  is_fn = true;
  docstring = new hydra_string("Combines functions into an effective function");
  type->arg_list.push_front(new type_gen_fn);
  type->rest_type = new type_nil;
}
hydra_object *op_addfn::call(hydra_object *alist, runtime &r, lexical_scope &s) {
  list<hydra_object*> arg_list = get_arg_list(alist, r, s);
  combined_fn* f = dynamic_cast<combined_fn*>(arg_list.front());
  arg_list.pop_front();

  for (hydra_object* o : arg_list) {
    hydra_oper* op = hydra_cast<hydra_oper>(o);
    f->add(op);
  }

  return (hydra_object*) f;
}

op_exit::op_exit() {
  is_fn = false;
  docstring = new hydra_string("Exits the current running application");
}

hydra_object *op_exit::call(hydra_object *alist, runtime &r, lexical_scope &s) {
  for (hydra_object* obj : hydra_object::node_list) {
    delete obj;
  }
  exit(0);
}

op_ref::op_ref() {
  is_fn = true;
  docstring = new hydra_string("Returns a reference to the provided value");
}

hydra_object *op_ref::call(hydra_object *alist, runtime &r, lexical_scope &s) {
  list<hydra_object*> arg_list = get_arg_list(alist, r, s);

  hydra_ref* ref = new hydra_ref;
  ref->ptr = arg_list.front();
  return ref;
}

op_var::op_var() {
  is_fn = true;
  docstring = new hydra_string("Creates some mutable storage, and returns a handle to it");
}

hydra_object *op_var::call(hydra_object *alist, runtime &r, lexical_scope &s) {
  list<hydra_object*> arg_list = get_arg_list(alist, r, s);

  hydra_var* var = new hydra_var;
  // TODO: add get-var function to objects?
  var->val = arg_list.front();
  return var;
}
