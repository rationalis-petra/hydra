#ifndef __HYDRA_TYPE_TYPE_HPP
#define __HYDRA_TYPE_TYPE_HPP

#include <list>
#include <string>
#include <vector>

#include "expressions/object.hpp"
#include "expressions/operation.hpp"
#include "expressions/user_object.hpp"

namespace type {

struct Type : public virtual expr::Object {
public:
  virtual expr::Object *check_type(expr::Object *obj) = 0;
  virtual expr::Object *subtype(Type *ty) = 0;
  virtual expr::Object *equal(Type *ty);
};

struct Symbol : public Type {
  virtual void mark_node();
  std::string to_string(expr::LocalRuntime &r, expr::LexicalScope& s);
  expr::Object *check_type(expr::Object *obj);
  virtual expr::Object *subtype(Type *ty);
};

// TODO: TUPLE, UNION!!!

struct Cons : public Type {
  Cons();
  Cons(Type *tcar, Type *tcdr);
  virtual void mark_node();
  std::string to_string(expr::LocalRuntime &r, expr::LexicalScope& s);
  expr::Object *check_type(expr::Object *obj);
  virtual expr::Object *subtype(Type *ty);

  Type *type_car;
  Type *type_cdr;
};

struct Vector : public Type {
  Vector();
  Vector(Type* t);
  Vector(Type* t, int i);

  virtual void mark_node();
  std::string to_string(expr::LocalRuntime &r, expr::LexicalScope& s);
  expr::Object *check_type(expr::Object *obj);
  virtual expr::Object *subtype(Type *ty);

  Type *type_elt;
};

struct List : public Type {
  virtual void mark_node();
  std::string to_string(expr::LocalRuntime &r, expr::LexicalScope& s);
  expr::Object *check_type(expr::Object *obj);
  virtual expr::Object *subtype(Type *ty);

  Type *elt_type;
};

struct Tuple : public Type {
  Tuple();
  Tuple(std::vector<Type *> types);
  virtual void mark_node();
  std::string to_string(expr::LocalRuntime &r, expr::LexicalScope& s);
  expr::Object *check_type(expr::Object *obj);
  virtual expr::Object *subtype(Type *ty);

  std::vector<Type *> types;
};

struct Union : public Type {
  virtual void mark_node();
  std::string to_string(expr::LocalRuntime &r, expr::LexicalScope& s);
  expr::Object *check_type(expr::Object *obj);
  Type *constructor(std::list<expr::Object *> lst);

  virtual expr::Object *subtype(Type *ty);
  std::list<Type *> types;
};

struct DerivesConstructor {
  void mark_node();
  std::string to_string(expr::LocalRuntime &r, expr::LexicalScope& s);
  Type *constructor(std::list<expr::Object *> lst);
};

struct IsConstructor : public expr::Operator {
  void mark_node();
  expr::Object *object;
  std::string to_string(expr::LocalRuntime &r, expr::LexicalScope& s);
  Type *constructor(std::list<expr::Object *> lst);
};

struct Derives : public Type {
  Derives(Object *obj);
  void mark_node();
  expr::Object *object;
  std::string to_string(expr::LocalRuntime &r, expr::LexicalScope& s);
  virtual expr::Object *subtype(Type *ty);
  virtual expr::Object *check_type(expr::Object *obj);
};

struct Is : public Type {
  Is(expr::Object *v);
  void mark_node();
  expr::Object *object;
  std::string to_string(expr::LocalRuntime &r, expr::LexicalScope& s);
  virtual expr::Object *subtype(Type *ty);
  virtual expr::Object *check_type(expr::Object *obj);
};

} // namespace type
#endif
