#ifndef __HYDRA_TYPE_TYPE_HPP
#define __HYDRA_TYPE_TYPE_HPP

#include <list>
#include <vector>
#include <string>

#include "expressions/object.hpp"
#include "expressions/oobject.hpp"
#include "expressions/operation.hpp"

namespace type {

struct Type : public virtual expr::Value {
public:
  virtual expr::Value *check_type(expr::Value *obj) = 0;
};

struct TypeConstructor : public virtual expr::Value {
public:
  virtual Type* constructor(std::list<expr::Value*> lst) = 0;
};

struct Symbol : public Type {
  virtual void mark_node();
  std::string to_string() const;
  expr::Value* check_type(expr::Value* obj);
};

// TODO: TUPLE, UNION!!!

struct Cons : public Type, public TypeConstructor {
  virtual void mark_node();
  std::string to_string() const;
  expr::Value* check_type(expr::Value* obj);
  Type* constructor(std::list<expr::Value*> lst);

  Type* type_car;
  Type* type_cdr;
};

struct Vector : public Type, public TypeConstructor {
  virtual void mark_node();
  std::string to_string() const;
  expr::Value* check_type(expr::Value* obj);
  Type* constructor(std::list<expr::Value*> lst);

  Type* type_elt;
};

struct List : public Type, public TypeConstructor {
  virtual void mark_node();
  std::string to_string() const;
  expr::Value* check_type(expr::Value* obj);
  Type* constructor(std::list<expr::Value*> lst);

  Type* elt_type;
};

struct Tuple : public Type, public TypeConstructor {
  virtual void mark_node();
  std::string to_string() const;
  expr::Value* check_type(expr::Value* obj);
  Type* constructor(std::list<expr::Value*> lst);

  std::vector<Type*> types;
};

struct Union : public Type, public TypeConstructor {
  virtual void mark_node();
  std::string to_string() const;
  expr::Value* check_type(expr::Value* obj);
  Type* constructor(std::list<expr::Value*> lst);

  std::list<Type*> types;
};

struct DerivesConstructor : public TypeConstructor {
  void mark_node();
  std::string to_string() const;
  Type* constructor(std::list<expr::Value*> lst);
};

struct Derives : public Type {
  Derives();
  void mark_node();
  expr::Object* object;
  std::string to_string() const;
  virtual expr::Value *check_type(expr::Value *obj);
};

struct EqlConstructor : public TypeConstructor{
  void mark_node();
  expr::Value* object;
  std::string to_string() const;
  Type* constructor(std::list<expr::Value*> lst);
};

struct Eql : public Type {
  Eql(expr::Value* v);
  void mark_node();
  expr::Value* object;
  std::string to_string() const;
  virtual expr::Value *check_type(expr::Value *obj);
};

}
#endif
