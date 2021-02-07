#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <typeinfo>

#include "expressions.hpp"
#include "operations.hpp"

using std::list;
using std::map;
using std::string;
using std::function;

string hydra_oper::to_string() const {
  return "<inbuilt operation>";
}

hydra_oper::hydra_oper() {
  type = new type_fn;
  docstring = new hydra_string;
}

void hydra_oper::mark_node() {
  if (marked) return;
  marked = true;
  docstring->mark_node();
  type->mark_node();
}

list<hydra_object *> hydra_oper::get_arg_list(hydra_object *arg_list,
                                              runtime &r, lexical_scope &s) {
  list<hydra_object *> out_list;
  while (!arg_list->null()) {
    hydra_cons *list_elt = dynamic_cast<hydra_cons *>(arg_list);
    hydra_object *arg = list_elt->car;
    if (is_fn) {
      out_list.push_back(arg->eval(r, s));
    } else {
      out_list.push_back(arg);
    }
    arg_list = list_elt->cdr;
  }
  if (type->check_args(out_list)->null()) {
    string err = "type check failed!";
    throw err;
  }
  return out_list;
}


// COMBINED FUNCTION

string combined_fn::to_string() const { return "<combined function>"; }

void combined_fn::mark_node() {
  if (marked) return;
  marked = true;
  type->mark_node();
  docstring->mark_node();
  for (hydra_oper* o : functions) {
    o->mark_node();
  }
}

void combined_fn::add(hydra_oper* fn) {
  if (combined_fn *f = dynamic_cast<combined_fn*>(fn)) {
    for (hydra_oper* o : f->functions) {
      add(o);
    }
  } else if (fn->is_fn) {
    functions.push_front(fn);
  } else {
    string err = "Attempted to add macro to add-fn!";
    throw err;
  }
}

hydra_object* combined_fn::call(hydra_object* alist, runtime &r, lexical_scope &s) {
  list<hydra_object*> arg_list = get_arg_list(alist, r, s);

  for (hydra_oper* fn : functions) {
    if (!fn->type->check_args(arg_list)->null()) {
      return fn->call(alist, r, s);
    }
  }
  string err = "No matching function in combined function";
  throw err;
}

