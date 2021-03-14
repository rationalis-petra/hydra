#ifndef __TYPE_BASIC_HPP
#define __TYPE_BASIC_HPP

#include <list>

#include "types/type.hpp"

namespace type {


struct UserObject : public Type {
  virtual void mark_node();
  std::string to_string() const;
  virtual expr::Object *check_type(expr::Object *obj);
  virtual expr::Object *subtype(Type *obj);
};

struct Nil : public Type {
  virtual void mark_node();
  std::string to_string() const;
  virtual expr::Object *check_type(expr::Object *obj);
  virtual expr::Object *subtype(Type *obj);
};

struct Any : public Type {
  virtual void mark_node();
  std::string to_string() const;
  virtual expr::Object *check_type(expr::Object *obj);
  virtual expr::Object *subtype(Type *obj);
};

struct Integer : public Type {
  virtual void mark_node();
  std::string to_string() const;
  expr::Object *check_type(expr::Object *obj);
  virtual expr::Object *subtype(Type *obj);
};

struct TString : public Type {
  virtual void mark_node();
  std::string to_string() const;
  expr::Object *check_type(expr::Object *obj);
  virtual expr::Object *subtype(Type *obj);
};

struct Module : public Type {
  virtual void mark_node();
  std::string to_string() const;
  expr::Object *check_type(expr::Object *obj);
  virtual expr::Object *subtype(Type *obj);
};

struct Char : public Type {
  virtual void mark_node();
  std::string to_string() const;
  expr::Object *check_type(expr::Object *obj);
  virtual expr::Object *subtype(Type *obj);
};

struct IOStream : public Type {
  virtual void mark_node();
  std::string to_string() const;
  expr::Object *check_type(expr::Object *obj);
  virtual expr::Object *subtype(Type *obj);
};
struct Istream : public Type {
  virtual void mark_node();
  std::string to_string() const;
  expr::Object *check_type(expr::Object *obj);
  virtual expr::Object *subtype(Type *obj);
};

struct Ostream : public Type {
  virtual void mark_node();
  std::string to_string() const;
  expr::Object *check_type(expr::Object *obj);
  virtual expr::Object *subtype(Type *obj);
};

struct MetaType : public Type {
  virtual void mark_node();
  std::string to_string() const;
  expr::Object *check_type(expr::Object *obj);
  virtual expr::Object *subtype(Type *obj);
};

struct MetaConstructor : public Type {
  virtual void mark_node();
  std::string to_string() const;
  expr::Object *check_type(expr::Object *obj);
  virtual expr::Object *subtype(Type *obj);
};

} // namespace type
#endif
