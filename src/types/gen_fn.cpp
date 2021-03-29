#include <string>
#include <iostream>

#include "expressions.hpp"
#include "types.hpp"
#include "utils.hpp"

using namespace interp;

using std::string;

using namespace type;

void GenFn::mark_node() {
  if (marked) return;
  Object::mark_node(); for (Type* t : arg_list) {
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
  rest_type = new Any;
  return_type = new Any;
}

std::string GenFn::to_string(LocalRuntime &r, LexicalScope &s) {
  //return string("{type (") + ") -> " + return_type->to_string() + "}";
  string str = "Gen Fn (";
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

expr::Object *GenFn::check_type(expr::Object *obj) {
  if (dynamic_cast<expr::GenericFn*>(obj)) {
    return expr::t::get();
  }
  return expr::nil::get();
}


expr::Object* GenFn::subtype(Type* other) {
  if (GenFn* tfn = dynamic_cast<GenFn*>(other)) {
    if (arg_list.size() == tfn->arg_list.size()) {
      auto our_ir = arg_list.begin();
      auto their_ir = tfn->arg_list.begin();
      // all of our arguments have to be subtypes of theirs
      while (our_ir != arg_list.end()) {
        if ((*their_ir)->subtype(*our_ir)->null()) {

          return expr::nil::get(); 
        }
        their_ir++;
        our_ir++;
      }

      // our return type must be a supertype of theirs
      // i.e. their return type must be a subtype of ours
      if (return_type->subtype(tfn->return_type)) {
        return expr::t::get();
      }
    }
  }
  return expr::nil::get();
}
