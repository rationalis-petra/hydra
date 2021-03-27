#include <string>

#include "operations.hpp"
#include "expressions.hpp"
#include "types.hpp"
#include "utils.hpp"

using namespace interp;

using std::string;
using std::list;

using expr::Object;
using type::Fn;
using type::Type;

Fn::Fn() {
  rest_type = nullptr;
  return_type = new type::Any;
  invoker = op::mk_fn_type;
}

void Fn::mark_node() {
  if (marked) return;
  Object::mark_node();
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
  if (return_type) {
    return_type->mark_node();
  }
}

std::string Fn::to_string(LocalRuntime &r, LexicalScope &s) {
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

Object *Fn::check_type(Object *obj) {
  if (dynamic_cast<expr::Operator*>(obj)) {
    return expr::t::get();
  }
  return expr::nil::get();
}


Object *Fn::check_args(list<Object*> alist) {
  for (auto type : arg_list) {
    if (alist.empty()) {
      throw new TooFewException;
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
    throw new TooManyException;
  }
  // now, we check keyword argumnets
  return expr::t::get();
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

#include <iostream>
expr::Object* Fn::subtype(Type* other) {
  if (Fn* tfn = dynamic_cast<Fn*>(other)) {
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

expr::Object* Fn::equal(Type* other) {
  Fn* fn2 = dynamic_cast<Fn*>(other);
  if (!fn2) 
    return expr::nil::get();

  if (rest_type && fn2->rest_type) {
    if (fn2->rest_type->equal(rest_type)->null())
      return expr::nil::get();
  }
  if (fn2->return_type->equal(return_type)->null())
    return expr::nil::get();
  if (arg_list.size() != fn2->arg_list.size()) 
    return expr::nil::get();
  if (optional_list.size() != fn2->optional_list.size()) 
    return expr::nil::get();

  auto it1 = arg_list.begin();
  auto it2 = fn2->arg_list.begin();
  while (it1 != arg_list.end()) {
    if ((*it1)->equal(*it2)->null()) 
      return expr::nil::get();
    it1++;
    it2++;
  }
  it1 = optional_list.begin();
  it2 = fn2->optional_list.begin();
  while (it1 != optional_list.end()) {
    if ((*it1)->equal(*it2)->null()) 
      return expr::nil::get();
    it1++;
    it2++;
  }

  return expr::t::get();
}
