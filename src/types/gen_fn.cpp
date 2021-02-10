#include <string>
#include <iostream>

#include "expressions.hpp"
#include "types.hpp"

using std::string;
using std::list;

void type_gen_fn::mark_node() {
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

type_gen_fn::type_gen_fn() {
  rest_type = nullptr;
  return_type = new type_nil;
}

std::string type_gen_fn::to_string() const {
  //return string("{type (") + ") -> " + return_type->to_string() + "}";
  string str = "Gen Fn (";
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

hydra_object *type_gen_fn::check_type(hydra_object *obj) {
  if (dynamic_cast<combined_fn*>(obj)) {
    return hydra_t::get();
  }
  return hydra_nil::get();
}

