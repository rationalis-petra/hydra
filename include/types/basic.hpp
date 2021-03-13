#ifndef __TYPE_BASIC_HPP
#define __TYPE_BASIC_HPP

#include <list>

#include "types/type.hpp"

namespace type {


struct UserObject : public Type {
  virtual void mark_node();
  std::string to_string() const;
  virtual expr::Object *check_type(expr::Object *obj);
};

struct Nil : public Type {
  virtual void mark_node();
  std::string to_string() const;
  virtual expr::Object *check_type(expr::Object *obj);
};

struct Any : public Type {
  virtual void mark_node();
  std::string to_string() const;
  virtual expr::Object *check_type(expr::Object *obj);
};

struct Integer : public Type {
  virtual void mark_node();
  std::string to_string() const;
  expr::Object *check_type(expr::Object *obj);
};

struct TString : public Type {
  virtual void mark_node();
  std::string to_string() const;
  expr::Object *check_type(expr::Object *obj);
};

struct Module : public Type {
  virtual void mark_node();
  std::string to_string() const;
  expr::Object *check_type(expr::Object *obj);
};

struct Char : public Type {
  virtual void mark_node();
  std::string to_string() const;
  expr::Object *check_type(expr::Object *obj);
};

struct IOStream : public Type {
  virtual void mark_node();
  std::string to_string() const;
  expr::Object *check_type(expr::Object *obj);
};
struct Istream : public Type {
  virtual void mark_node();
  std::string to_string() const;
  expr::Object *check_type(expr::Object *obj);
};

struct Ostream : public Type {
  virtual void mark_node();
  std::string to_string() const;
  expr::Object *check_type(expr::Object *obj);
};

struct MetaType : public Type {
  virtual void mark_node();
  std::string to_string() const;
  expr::Object *check_type(expr::Object *obj);
};

struct MetaConstructor : public Type {
  virtual void mark_node();
  std::string to_string() const;
  expr::Object *check_type(expr::Object *obj);
};

} // namespace type
#endif
