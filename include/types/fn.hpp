#ifndef __HYDRA_TYPE_FN_HPP
#define __HYDRA_TYPE_FN_HPP

#include <string>
#include <list>

#include "expressions.hpp"
#include "operations.hpp"

#include "types/type.hpp"

namespace type {
struct Fn : public Type {
  virtual void mark_node();

  Fn();

  // factory methods
  static Fn* with_rest(Type* tp);
  static Fn* with_args(std::vector<Type*> args);
  static Fn* with_args_optional(std::vector<Type*> args, std::vector<Type*> opts);
  static Fn* with_all(std::vector<Type*> args, Type* rest, Type* ret);

  std::string to_string() const;
  expr::Value* check_type(expr::Value* obj);
  expr::Value* check_args(std::list<expr::Value*> alist);

  Type* constructor(std::list<expr::Value*> lst);

  Type* return_type;
  Type* rest_type;
  std::vector<Type*> arg_list;
  std::vector<Type*> optional_list;
  std::list<expr::Symbol*> keyword_names;
  std::list<Type*> keyword_list;
};

struct Mac : public Fn {
  virtual void mark_node();

  std::string to_string() const;
  expr::Value* check_type(expr::Value* obj);
  expr::Value* check_args(std::list<expr::Value*> alist);
  Type* constructor(std::list<expr::Value*> lst);
};

struct GenFn : public Type {
  virtual void mark_node();

  GenFn();
  std::string to_string() const;
  expr::Value* check_type(expr::Value* obj);
  Type* constructor(std::list<expr::Value*> lst);

  Type* return_type;
  Type* rest_type;
  std::list<Type*> arg_list;
  std::list<Type*> optional_list;
  std::list<expr::Symbol*> keyword_names;
  std::list<Type*> keyword_list;
};

}
#endif
