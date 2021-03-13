#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <typeinfo>

#include "expressions.hpp"
#include "operations.hpp"

using std::list;
using std::string;

using namespace expr;

string Operator::to_string() const { return "<inbuilt operation>"; }

Operator::Operator() {
  type = new type::Fn;
  docstring = new HString;
}

void Operator::mark_node() {
  if (marked)
    return;
  marked = true;
  docstring->mark_node();
  type->mark_node();
}

list<Object *> Operator::get_arg_list(Object *arg_list, LocalRuntime &r,
                                     LexicalScope &s) {
  // ASSUME that arg_list is rooted
  Object *original_list = arg_list;

  list<Object *> out_list;
  while (!arg_list->null()) {
    Cons *list_elt = dynamic_cast<Cons *>(arg_list);
    Object *arg = list_elt->car;
    if (is_fn) {
      out_list.push_back(arg->eval(r, s));
    } else {
      out_list.push_back(arg);
    }

    // we just added a value to the arg_list: we need to root it
    // This means that a value will be 'double-rooted' for macros,
    // but better safe than sorry!
    Object::roots.insert(out_list.back());
    arg_list = list_elt->cdr;
  }
  if (type->check_args(out_list)->null()) {
    string err =
        "type check failed! for arg_list: " + original_list->to_string() +
        "expected type: " + type->to_string();
    throw err;
  }
  return out_list;
}

// COMBINED FUNCTION

string CombinedFn::to_string() const { return "<generic function>"; }

void CombinedFn::mark_node() {
  if (marked)
    return;

  marked = true;
  type->mark_node();
  docstring->mark_node();
  for (Operator *o : functions) {
    o->mark_node();
  }
}

void CombinedFn::add(Operator *fn) {
  if (CombinedFn *f = dynamic_cast<CombinedFn *>(fn)) {
    for (Operator *o : f->functions) {
      add(o);
    }
  } else if (fn->is_fn) {
    functions.push_front(fn);
  } else {
    string err = "Attempted to add macro to add-fn!";
    throw err;
  }
}

Object *CombinedFn::call(Object *alist, LocalRuntime &r, LexicalScope &s) {
  // ASSUME that the function itself is marked

  // All values in the arg_list are rooted by the get_arg_list function
  list<Object *> arg_list = get_arg_list(alist, r, s);

  for (Operator *fn : functions) {
    if (!fn->type->check_args(arg_list)->null()) {
      Object* out =  fn->call(alist, r, s);

      // we need to unroot values in the arg_list
      for (Object* v : arg_list) {
        Object::roots.remove(v);
      }
      return out;
    }
  }
  string err = "No matching function in combined function";
  throw err;
}

InbuiltOperator::InbuiltOperator(string _docstring,
                                 Object *(*_fnc)(Operator *, Object *,
                                                LocalRuntime &r,
                                                LexicalScope &s),
                                 type::Fn *t, bool isfn) {
  fnc = _fnc;
  docstring = new HString(_docstring);
  type = t;
  is_fn = isfn;
}

Object *InbuiltOperator::call(Object *arg_list, LocalRuntime &r,
                             LexicalScope &s) {
  // ASSUME arg_list is rooted
  // WE delegate rooting to inbuilt functions
  return fnc(this, arg_list, r, s);
}
