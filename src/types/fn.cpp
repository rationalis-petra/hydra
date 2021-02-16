#include <string>
#include <iostream>

#include "expressions.hpp"
#include "types.hpp"

using std::string;
using std::list;

void type_fn::mark_node() {
  if (marked) return;
  marked = true;
  for (hydra_type* t : arg_list) {
    t->mark_node();
  }
  for (hydra_type* t : optional_list) {
    t->mark_node();
  }
  for (hydra_type* t : keyword_list) {
    t->mark_node();
  }
  for (hydra_symbol* t : keyword_names) {
    t->mark_node();
  }
  if (rest_type) {
    rest_type->mark_node();
  }
}

type_fn::type_fn() {
  rest_type = nullptr;
  return_type = new type_nil;
}

std::string type_fn::to_string() const {
  //return string("{type (") + ") -> " + return_type->to_string() + "}";
  string str = "fn (";
  bool once = false;
  for (hydra_type* t : arg_list) {
    str += once ? " " : (once = true, "");
    str += t->to_string();
  }
  if (rest_type) {
    str += once ? " " : (once = true, "");
    str += ":rest " + rest_type->to_string();
  }
  str += ") -> " + return_type->to_string();
  return str;
}

hydra_object *type_fn::check_type(hydra_object *obj) {
  if (dynamic_cast<hydra_oper*>(obj)) {
    return hydra_t::get();
  }
  return hydra_nil::get();
}

hydra_object *type_fn::check_args(list<hydra_object*> alist) {
  for (auto type : arg_list) {
    if (alist.empty()) {
      string err = "Too few arguments to function";
      throw err;
    }
    if (type->check_type(alist.front())->null()) {
      return hydra_nil::get();
    }
    alist.pop_front();
  }
  // now, we check optional argumnets
  for (auto type : optional_list) {
    if (alist.empty()) {
      break;
    }
    if (type->check_type(alist.front())->null()) {
      return hydra_nil::get();
    }
    alist.pop_front();
  }

  if (rest_type) {
    for (hydra_object* obj : alist) {
      if (rest_type->check_type(obj)->null()) {
        return hydra_nil::get();
      }
    }
    alist.clear();
  }
  if (alist.size() != 0) {
    string err = "Too many arguments to function" + this->to_string();
    throw err;
  }
  // now, we check keyword argumnets
  return hydra_t::get();
}


hydra_type *type_fn::constructor(list<hydra_object*> lst) {
  if (lst.size() == 0) {
    return this;
  } else {
    string err = "type_fn::constructor unimplemented";
    throw err;
  }
}
