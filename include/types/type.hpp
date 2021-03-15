#ifndef __HYDRA_TYPE_TYPE_HPP
#define __HYDRA_TYPE_TYPE_HPP

#include <list>
#include <vector>
#include <string>

#include "expressions/object.hpp"
#include "expressions/oobject.hpp"
#include "expressions/operation.hpp"

namespace type {

struct Type : public virtual expr::Object {
public:
  virtual expr::Object *check_type(expr::Object *obj) = 0;
  virtual expr::Object *subtype(Type *ty) = 0;
  virtual expr::Object *equal(Type* ty);
};

struct TypeConstructor : public virtual expr::Object {
public:
  virtual Type* constructor(std::list<expr::Object*> lst) = 0;
};

struct Symbol : public Type {
  virtual void mark_node();
  std::string to_string() const;
  expr::Object* check_type(expr::Object* obj);
  virtual expr::Object *subtype(Type *ty);
};

// TODO: TUPLE, UNION!!!

struct Cons : public Type, public TypeConstructor {
  virtual void mark_node();
  std::string to_string() const;
  expr::Object* check_type(expr::Object* obj);
  virtual expr::Object *subtype(Type *ty);
  Type* constructor(std::list<expr::Object*> lst);

  Type* type_car;
  Type* type_cdr;
};

struct Vector : public Type, public TypeConstructor {
  virtual void mark_node();
  std::string to_string() const;
  expr::Object* check_type(expr::Object* obj);
  virtual expr::Object *subtype(Type *ty);
  Type* constructor(std::list<expr::Object*> lst);

  Type* type_elt;
};

struct List : public Type, public TypeConstructor {
  virtual void mark_node();
  std::string to_string() const;
  expr::Object* check_type(expr::Object* obj);
  virtual expr::Object *subtype(Type *ty);
  Type* constructor(std::list<expr::Object*> lst);

  Type* elt_type;
};

struct Tuple : public Type, public TypeConstructor {
  Tuple();
  Tuple(std::vector<Type*> types);
  virtual void mark_node();
  std::string to_string() const;
  expr::Object* check_type(expr::Object* obj);
  virtual expr::Object *subtype(Type *ty);
  Type* constructor(std::list<expr::Object*> lst);

  std::vector<Type*> types;
};

struct Union : public Type, public TypeConstructor {
  virtual void mark_node();
  std::string to_string() const;
  expr::Object* check_type(expr::Object* obj);
  Type* constructor(std::list<expr::Object*> lst);

  virtual expr::Object *subtype(Type *ty);
  std::list<Type*> types;
};

struct DerivesConstructor : public TypeConstructor {
  void mark_node();
  std::string to_string() const;
  Type* constructor(std::list<expr::Object*> lst);
};

struct Derives : public Type {
  Derives(Object* obj);
  void mark_node();
  expr::Object* object;
  std::string to_string() const;
  virtual expr::Object *subtype(Type *ty);
  virtual expr::Object *check_type(expr::Object *obj);
};

struct EqlConstructor : public TypeConstructor{
  void mark_node();
  expr::Object* object;
  std::string to_string() const;
  Type* constructor(std::list<expr::Object*> lst);
};

struct Eql : public Type {
  Eql(expr::Object* v);
  void mark_node();
  expr::Object* object;
  std::string to_string() const;
  virtual expr::Object *subtype(Type *ty);
  virtual expr::Object *check_type(expr::Object *obj);
};

}
#endif
