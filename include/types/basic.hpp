#ifndef __TYPE_BASIC_HPP
#define __TYPE_BASIC_HPP

#include <list>

#include "types/type.hpp"
#include "interpreter.hpp"

namespace type {

  extern Type* number_type;
  extern Type* integer_type;
  extern Type* float_type;

  extern Type* nil_type;
  extern Type* character_type;
  extern Type* module_type;
  extern Type* string_type;
  extern Type* istream_type;
  extern Type* ostream_type;
  extern Type* iostream_type;
  extern Type* mirror_type;
  extern Type* meta_type;
  extern Type* symbol_type;

  extern Type* common_type;
  extern Type* default_type;

  void initialize_types();

struct Nil : public Type {
  virtual void mark_node();
  std::string to_string(interp::LocalRuntime &r, interp::LexicalScope& s);
  virtual expr::Object *check_type(expr::Object *obj);
  virtual expr::Object *subtype(Type *obj);
};

struct Any : public Type {
  virtual void mark_node();
  std::string to_string(interp::LocalRuntime &r, interp::LexicalScope& s);
  virtual expr::Object *check_type(expr::Object *obj);
  virtual expr::Object *subtype(Type *obj);
};

struct MetaType : public Type {
  virtual void mark_node();
  std::string to_string(interp::LocalRuntime &r, interp::LexicalScope& s);
  expr::Object *check_type(expr::Object *obj);
  virtual expr::Object *subtype(Type *obj);
};

struct MetaConstructor : public Type {
  virtual void mark_node();
  std::string to_string(interp::LocalRuntime &r, interp::LexicalScope& s);
  expr::Object *check_type(expr::Object *obj);
  virtual expr::Object *subtype(Type *obj);
};

} // namespace type
#endif
