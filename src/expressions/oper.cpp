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
using namespace interp;

string Operator::to_string(LocalRuntime &r, LexicalScope &s) {
  return "<inbuilt operation>";
}

Object *Operator::operator()(std::list<Object *> arg_list, LocalRuntime &r,
                             LexicalScope &s, bool macexpand) {
  return call(arg_list, r, s, macexpand);
}


Operator::Operator() {
  type = new type::Fn;
  Symbol* dstring = get_keyword("docstring");
  metadata[dstring] = new HString;
  invoker = this;
}

void Operator::mark_node() {
  if (marked) return;
  Object::mark_node();
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
      Object::collector->insert_root(out_list.back());
    }
  } else {
    out_list = alist;
    // TODO: make me able to remove this!!
    for (Object* arg : alist) {
      Object::collector->insert_root(arg);
    }
  }
  try {
    if (type->check_args(out_list)->null()) {
      string ierr = "type check failed! for arg_list: " +
                    hydra_to_string(original_list, r, s) +
                    " in function: " + hydra_to_string(this, r, s) +
                    "\nexpected type: " + hydra_to_string(type, r, s);
      throw ierr;
    }
  } catch (TooFewException *e) {
    string ierr = "type check failed! for arg_list: " +
                  hydra_to_string(original_list, r, s) +
                  "too few arguments to function " + hydra_to_string(this, r, s);
    throw ierr;

  } catch (TooManyException *e) {
    string ierr = "type check failed! for arg_list: " +
      hydra_to_string(original_list, r, s) +
                  "too many arguments to function " + hydra_to_string(this, r, s);
    throw ierr;
  }
  return out_list;
}


InbuiltOperator::InbuiltOperator(string _name, string _docstring,
                                 Object *(*_fnc)(std::list<Object *> arg_list,
                                                LocalRuntime &r,
                                                LexicalScope &s),
                                 type::Fn *t, bool isfn) {
  name = _name;
  fnc = _fnc;
  Symbol* dstring = get_keyword("docstring");
  metadata[dstring] = new HString(_docstring);
  type = t;
  is_fn = isfn;
}

Object *InbuiltOperator::call(std::list<Object *> arg_list, LocalRuntime &r,
                              LexicalScope &s, bool b) {
  // ASSUME arg_list is rooted
  // WE delegate rooting to inbuilt functions
  return fnc(arg_list, r, s);
}

string InbuiltOperator::to_string(LocalRuntime &r, LexicalScope &s) {
  return name;
}
