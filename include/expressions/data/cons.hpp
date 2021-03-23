#ifndef __HYDRA_EXPRESSIONS_DATA_CONS_HPP
#define __HYDRA_EXPRESSIONS_DATA_CONS_HPP

#include <string>
#include <istream>
#include <list>
#include <vector>

#include "expressions/object.hpp"
#include "expressions/parent.hpp"

namespace expr {

struct Cons : public Object {
  virtual void mark_node();
  static Parent* parent;

  std::string to_string(LocalRuntime &r, LexicalScope& s); 
  Cons(Object* car, Object* cdr);
  Object* eval(LocalRuntime& r, LexicalScope& s);

  Object* car;
  Object* cdr;

  unsigned len();
};
}

#endif
