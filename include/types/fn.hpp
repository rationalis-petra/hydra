#ifndef __HYDRA_TYPE_FN_HPP
#define __HYDRA_TYPE_FN_HPP

#include <string>
#include <list>

#include "expressions/object.hpp"

#include "types/type.hpp"

namespace type {
  struct Fn : public Type {
  virtual void mark_node();

  Fn();
  virtual expr::Object *subtype(Type *obj);

  // factory methods
  static Fn* with_rest(Type* tp);
  static Fn* with_args(std::vector<Type*> args);
  static Fn* with_args_optional(std::vector<Type*> args, std::vector<Type*> opts);
  static Fn* with_all(std::vector<Type*> args, Type* rest, Type* ret);

  std::string to_string(interp::LocalRuntime &r, interp::LexicalScope& s);
  expr::Object* check_type(expr::Object* obj);
  expr::Object* check_args(std::list<expr::Object*> alist);
  virtual expr::Object *equal(Type* ty);

    Type* constructor(std::list<expr::Object*> lst, interp::LocalRuntime& r, interp::LexicalScope&s);

  Type* return_type;
  Type* rest_type;
  std::vector<Type*> arg_list;
  std::vector<Type*> optional_list;
  std::list<expr::Symbol*> keyword_names;
  std::list<Type*> keyword_list;
};

struct Mac : public Fn {
  virtual void mark_node();
  virtual expr::Object *subtype(Type *obj);

  std::string to_string(interp::LocalRuntime &r, interp::LexicalScope& s);
  expr::Object* check_type(expr::Object* obj);
  expr::Object* check_args(std::list<expr::Object*> alist);
  Type* constructor(std::list<expr::Object*> lst);
};

struct GenFn : public Fn {
  virtual void mark_node();
  virtual expr::Object *subtype(Type *obj);

  GenFn();
  std::string to_string(interp::LocalRuntime &r, interp::LexicalScope& s);
  expr::Object* check_type(expr::Object* obj);
  Type* constructor(std::list<expr::Object*> lst);
};

}
#endif
