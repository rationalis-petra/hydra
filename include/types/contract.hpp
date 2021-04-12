
#ifndef __TYPE_CONTRACT_HPP
#define __TYPE_CONTRACT_HPP

#include <list>

#include "types/type.hpp"
#include "interpreter.hpp"

namespace type {

  extern Type* Contract;

  void initialize_types();

struct Contract : public Type {
  // 
  std::list<Symbol*> parameters;

  virtual void mark_node();
  std::string to_string(interp::LocalRuntime &r, interp::LexicalScope& s);
  virtual expr::Object *check_type(expr::Object *obj);
  virtual expr::Object *subtype(Type *obj);
};
