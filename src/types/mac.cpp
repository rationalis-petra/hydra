
#include <string>
#include <iostream>

#include "expressions.hpp"
#include "types.hpp"

using std::string;
using std::list;

using expr::Object;
using type::Mac;
using type::Type;

void Mac::mark_node() {
  if (marked) return;
  marked = true;
  for (Type* t : arg_list) {
    t->mark_node();
  }
  for (Type* t : optional_list) {
    t->mark_node();
  }
  for (Type* t : keyword_list) {
    t->mark_node();
  }
  for (expr::Symbol* t : keyword_names) {
    t->mark_node();
  }
  if (rest_type) {
    rest_type->mark_node();
  }
}


std::string Mac::to_string() const {
  //return string("{type (") + ") -> " + return_type->to_string() + "}";
  string str = "fn (";
  bool once = false;
  for (Type* t : arg_list) {
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

Object *Mac::check_type(Object *obj) {
  // TODO: fix me!!
  if (dynamic_cast<expr::Operator*>(obj)) {
    return expr::t::get();
  }
  return expr::nil::get();
}

Object *Mac::check_args(list<Object*> alist) {
  for (auto type : arg_list) {
    if (alist.empty()) {
      string err = "Too few arguments to function";
      throw err;
    }
    if (type->check_type(alist.front())->null()) {
      return expr::nil::get();
    }
    alist.pop_front();
  }
  // now, we check optional argumnets
  for (auto type : optional_list) {
    if (alist.empty()) {
      break;
    }
    if (type->check_type(alist.front())->null()) {
      return expr::nil::get();
    }
    alist.pop_front();
  }

  if (rest_type) {
    for (Object* obj : alist) {
      if (rest_type->check_type(obj)->null()) {
        return expr::nil::get();
      }
    }
    alist.clear();
  }
  if (alist.size() != 0) {
    string err = "Too many arguments to function" + this->to_string();
    throw err;
  }
  // now, we check keyword argumnets
  return expr::t::get();
}


Type *Mac::constructor(list<Object*> lst) {
  if (lst.size() == 0) {
    return this;
  } else {
    string err = "type_mac::constructor unimplemented";
    throw err;
  }
}

