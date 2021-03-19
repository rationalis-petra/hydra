#ifndef __TYPE_BASIC_HPP
#define __TYPE_BASIC_HPP

#include <list>

#include "types/type.hpp"
#include "expressions/runtime.hpp"

namespace type {

  extern Type* integer_type;
  extern Type* nil_type;
  extern Type* character_type;
  extern Type* module_type;
  extern Type* string_type;
  extern Type* istream_type;
  extern Type* ostream_type;
  extern Type* iostream_type;
  extern Type* meta_type;

  void initialize_types();

struct UserObject : public Type {
  virtual void mark_node();
  std::string to_string(expr::LocalRuntime &r, expr::LexicalScope& s);
  virtual expr::Object *check_type(expr::Object *obj);
  virtual expr::Object *subtype(Type *obj);
};

struct Nil : public Type {
  virtual void mark_node();
  std::string to_string(expr::LocalRuntime &r, expr::LexicalScope& s);
  virtual expr::Object *check_type(expr::Object *obj);
  virtual expr::Object *subtype(Type *obj);
};

struct Any : public Type {
  virtual void mark_node();
  std::string to_string(expr::LocalRuntime &r, expr::LexicalScope& s);
  virtual expr::Object *check_type(expr::Object *obj);
  virtual expr::Object *subtype(Type *obj);
};

struct Module : public Type {
  virtual void mark_node();
  std::string to_string(expr::LocalRuntime &r, expr::LexicalScope& s);
  expr::Object *check_type(expr::Object *obj);
  virtual expr::Object *subtype(Type *obj);
};

struct IOStream : public Type {
  virtual void mark_node();
  std::string to_string(expr::LocalRuntime &r, expr::LexicalScope& s);
  expr::Object *check_type(expr::Object *obj);
  virtual expr::Object *subtype(Type *obj);
};

struct Istream : public Type {
  virtual void mark_node();
  std::string to_string(expr::LocalRuntime &r, expr::LexicalScope& s);
  expr::Object *check_type(expr::Object *obj);
  virtual expr::Object *subtype(Type *obj);
};

struct Ostream : public Type {
  virtual void mark_node();
  std::string to_string(expr::LocalRuntime &r, expr::LexicalScope& s);
  expr::Object *check_type(expr::Object *obj);
  virtual expr::Object *subtype(Type *obj);
};

struct MetaType : public Type {
  virtual void mark_node();
  std::string to_string(expr::LocalRuntime &r, expr::LexicalScope& s);
  expr::Object *check_type(expr::Object *obj);
  virtual expr::Object *subtype(Type *obj);
};

struct MetaConstructor : public Type {
  virtual void mark_node();
  std::string to_string(expr::LocalRuntime &r, expr::LexicalScope& s);
  expr::Object *check_type(expr::Object *obj);
  virtual expr::Object *subtype(Type *obj);
};

} // namespace type
#endif
