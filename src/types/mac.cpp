
#include <string>
#include <iostream>

#include "expressions.hpp"
#include "types.hpp"
#include "utils.hpp"

using std::string;
using std::list;

using expr::Object;
using type::Mac;
using type::Type;

using namespace interp;

void Mac::mark_node() {
  if (marked) return;
  Object::mark_node();
  for (Type* t : arg_list) {
    t->mark_node();
  }
  for (Type* t : optional_list) {
    t->mark_node();
  }
  for (auto it : keywords) {
    it.second->mark_node();
    it.first->mark_node();
  }
  if (rest_type) {
    rest_type->mark_node();
  }
}


std::string Mac::to_string(LocalRuntime &r, LexicalScope &s) {
  //return string("{type (") + ") -> " + return_type->to_string() + "}";
  string str = "fn (";
  bool once = false;
  for (Type* t : arg_list) {
    str += once ? " " : (once = true, "");
    str += hydra_to_string(t, r, s);
  }
  if (rest_type) {
    str += once ? " " : (once = true, "");
    str += ":rest " + hydra_to_string(rest_type, r, s);
  }
  str += ") -> " + hydra_to_string(return_type, r, s);
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
    string err = "Too many arguments to function (c++: types/mac.cpp)";
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

expr::Object* Mac::subtype(Type *obj) {
  string err = "subtyoe for Mac not implemetned!";
  throw err;
}
