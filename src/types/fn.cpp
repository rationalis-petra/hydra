#include <string>
#include <iostream>

#include "expressions.hpp"
#include "types.hpp"

using std::string;
using std::list;

using expr::Object;
using type::Fn;
using type::Type;

void Fn::mark_node() {
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

Fn::Fn() {
  rest_type = nullptr;
  return_type = new type::Any;
}

std::string Fn::to_string() const {
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

Object *Fn::check_type(Object *obj) {
  if (dynamic_cast<expr::Operator*>(obj)) {
    return expr::t::get();
  }
  return expr::nil::get();
}

Object *Fn::check_args(list<Object*> alist) {
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


Type *Fn::constructor(list<Object*> lst) {
  if (lst.size() == 0) {
    return this;
  } else {
    string err = "type_fn::constructor unimplemented";
    throw err;
  }
}

Fn *Fn::with_all(std::vector<Type *> args, Type *rest, Type *ret) {
  Fn* tfn = new Fn;
  tfn->arg_list = args;
  tfn->rest_type = rest;
  tfn->return_type = ret;
  return tfn; 
}

Fn *Fn::with_args(std::vector<Type *> args) {
  Fn* tfn = new Fn;
  tfn->arg_list = args;
  tfn->rest_type = nullptr;
  return tfn; 
}

Fn *Fn::with_rest(Type *tp) {
  Fn* tfn = new Fn;
  tfn->rest_type = tp;
  return tfn;
}

Fn *Fn::with_args_optional(std::vector<Type *> args, std::vector<Type *> opts) {
  Fn* tfn = new Fn;
  tfn->arg_list = args;
  tfn->optional_list = opts;
  tfn->rest_type = nullptr;
  return tfn;
}

expr::Object* Fn::subtype(Type* other) {
  if (Fn* tfn = dynamic_cast<Fn*>(other)) {
    if (arg_list.size() == tfn->arg_list.size()) {
      auto our_ir = arg_list.begin();
      auto their_ir = tfn->arg_list.begin();
      // all of our arguments have to be subtypes of theirs
      while (our_ir != arg_list.end()) {
        if (!(*their_ir)->subtype(*our_ir)) {
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
