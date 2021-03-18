#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <typeinfo>

#include "utils.hpp"
#include "expressions.hpp"
#include "operations.hpp"

using std::list;
using std::string;

using namespace expr;
string Operator::to_string() const { return "<inbuilt operation>"; }

Operator::Operator() {
  type = new type::Fn;
  docstring = new HString;
  invoker = this;
}

void Operator::mark_node() {
  if (marked) return;
  Object::mark_node();
  docstring->mark_node();
  type->mark_node();
}

list<Object *> Operator::get_arg_list(Object *arg_list, LocalRuntime &r,
                                     LexicalScope &s) {
  // ASSUME that arg_list is rooted
  Object *original_list = arg_list;

  list<Object*> alist = cons_to_list(arg_list);
  list<Object *> out_list;
  if (is_fn) {
    for (Object* arg : alist) {
      out_list.push_back(arg->eval(r, s));
      Object::roots.insert(out_list.back());
    }
  } else {
    out_list = alist;
    // TODO: make me able to remove this!!
    for (Object* arg : alist) {
      Object::roots.insert(arg);
    }
  }
  if (type->check_args(out_list)->null()) {
    string err =
        "type check failed! for arg_list: " + original_list->to_string() +
        "expected type: " + type->to_string();
    throw err;
  }
  return out_list;
}


InbuiltOperator::InbuiltOperator(string _docstring,
                                 Object *(*_fnc)(std::list<Object *> arg_list,
                                                LocalRuntime &r,
                                                LexicalScope &s),
                                 type::Fn *t, bool isfn) {
  fnc = _fnc;
  docstring = new HString(_docstring);
  type = t;
  is_fn = isfn;
}

Object *InbuiltOperator::call(std::list<Object *> arg_list, LocalRuntime &r,
                              LexicalScope &s, bool b) {
  // ASSUME arg_list is rooted
  // WE delegate rooting to inbuilt functions
  return fnc(arg_list, r, s);
}
