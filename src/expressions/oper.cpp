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

list<Value *> Operator::get_arg_list(Value *arg_list, LocalRuntime &r,
                                     LexicalScope &s) {
  Value *original_list = arg_list;
  list<Value *> out_list;
  while (!arg_list->null()) {
    Cons *list_elt = dynamic_cast<Cons *>(arg_list);
    Value *arg = list_elt->car;
    if (is_fn) {
      out_list.push_back(arg->eval(r, s));
    } else {
      out_list.push_back(arg);
    }
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

Value *CombinedFn::call(Value *alist, LocalRuntime &r, LexicalScope &s) {
  list<Value *> arg_list = get_arg_list(alist, r, s);

  for (Operator *fn : functions) {
    if (!fn->type->check_args(arg_list)->null()) {
      return fn->call(alist, r, s);
    }
  }
  string err = "No matching function in combined function";
  throw err;
}

InbuiltOperator::InbuiltOperator(string _docstring,
                                 Value *(*_fnc)(Operator *, Value *,
                                                LocalRuntime &r,
                                                LexicalScope &s),
                                 type::Fn *t, bool isfn) {
  fnc = _fnc;
  docstring = new HString(_docstring);
  type = t;
  is_fn = isfn;
}

Value *InbuiltOperator::call(Value *arg_list, LocalRuntime &r,
                             LexicalScope &s) {
  return fnc(this, arg_list, r, s);
}
