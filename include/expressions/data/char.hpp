#ifndef __HYDRA_EXPRESSIONS_DATA_CHAR_HPP
#define __HYDRA_EXPRESSIONS_DATA_CHAR_HPP

#include <istream>
#include <list>
#include <string>
#include <vector>

#include "expressions/object.hpp"
#include "expressions/parent.hpp"

namespace expr {

struct Char : public Object {
  virtual void mark_node();
  static Parent* parent;

  Char(int c);
  std::string to_string(interp::LocalRuntime &r, interp::LexicalScope& s);
  int value;
};

} // namespace expr

#endif
