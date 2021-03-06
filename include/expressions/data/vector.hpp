#ifndef __HYDRA_EXPRESSIONS_DATA_VECTOR_HPP
#define __HYDRA_EXPRESSIONS_DATA_VECTOR_HPP

#include <string>
#include <istream>
#include <list>
#include <vector>

#include "expressions/object.hpp"
#include "expressions/parent.hpp"

namespace expr {

struct Vector : public Object {
  // expose vector access
  Object* operator[](int i);
  int size();

  static Parent* parent;

  virtual void mark_node();

  std::string to_string(interp::LocalRuntime &r, interp::LexicalScope& s);
  std::vector<Object*> array;
};

}

#endif
