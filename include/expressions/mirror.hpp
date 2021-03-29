#ifndef __HYDRA_EXPRESSIONS_MIRROR_HPP
#define __HYDRA_EXPRESSIONS_MIRROR_HPP

#include <string>

#include "interpreter.hpp"
#include "expressions/object.hpp"
#include "expressions/parent.hpp"

namespace expr {

struct Mirror : public Object {
  Mirror(Object* reflectee);
  virtual void mark_node();

  std::string to_string(interp::LocalRuntime &r, interp::LexicalScope& s);
  Object* reflectee;

  static Object* parent;
};

} 

#endif
