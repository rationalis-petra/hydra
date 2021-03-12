#include <string>
#include <iostream>

#include "expressions.hpp"
#include "types.hpp"

using std::string;
using std::list;

using namespace type;

void GenFn::mark_node() {
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

GenFn::GenFn() {
  rest_type = nullptr;
  return_type = new Nil;
}

std::string GenFn::to_string() const {
  //return string("{type (") + ") -> " + return_type->to_string() + "}";
  string str = "Gen Fn (";
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

expr::Value *GenFn::check_type(expr::Value *obj) {
  if (dynamic_cast<expr::CombinedFn*>(obj)) {
    return expr::t::get();
  }
  return expr::nil::get();
}


Type *GenFn::constructor(list<expr::Value*> lst) {
  if (lst.size() == 0) {
    return this;
  } else {
    string err = "type_gen_fn::constructor not implemented";
    throw err;
  }
}
