#ifndef __TYPE_BASIC_HPP
#define __TYPE_BASIC_HPP

#include <list>

#include "types/type.hpp"

namespace type {

// struct Type : public expr::Value {
//   void mark_node();
//   std::string to_string() const;
//   virtual expr::Value *check_type(expr::Value *obj);
//   Type *constructor(std::list<expr::Value *> lst);
// };

struct Nil : public Type {
  virtual void mark_node();
  std::string to_string() const;
  virtual expr::Value *check_type(expr::Value *obj);
};

struct Integer : public Type {
  virtual void mark_node();
  std::string to_string() const;
  expr::Value *check_type(expr::Value *obj);
};

struct TString : public Type {
  virtual void mark_node();
  std::string to_string() const;
  expr::Value *check_type(expr::Value *obj);
};

struct Module : public Type {
  virtual void mark_node();
  std::string to_string() const;
  expr::Value *check_type(expr::Value *obj);
};

struct Char : public Type {
  virtual void mark_node();
  std::string to_string() const;
  expr::Value *check_type(expr::Value *obj);
};

struct IOStream : public Type {
  virtual void mark_node();
  std::string to_string() const;
  expr::Value *check_type(expr::Value *obj);
};

struct Istream : public Type {
  virtual void mark_node();
  std::string to_string() const;
  expr::Value *check_type(expr::Value *obj);
};

struct Ostream : public Type {
  virtual void mark_node();
  std::string to_string() const;
  expr::Value *check_type(expr::Value *obj);
};

struct MetaType : public Type {
  virtual void mark_node();
  std::string to_string() const;
  expr::Value *check_type(expr::Value *obj);
};

struct MetaConstructor : public Type {
  virtual void mark_node();
  std::string to_string() const;
  expr::Value *check_type(expr::Value *obj);
};

} // namespace type
#endif
