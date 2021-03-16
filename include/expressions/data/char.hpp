#ifndef __HYDRA_EXPRESSIONS_DATA_CHAR_HPP
#define __HYDRA_EXPRESSIONS_DATA_CHAR_HPP

#include <istream>
#include <list>
#include <string>
#include <vector>

#include "expressions/object.hpp"
#include "parent.hpp"

namespace expr {

struct Char : public Object {
  virtual void mark_node();
  static Parent* parent;

  Char();
  Char(int c);
  std::string to_string() const;
  int value;
};

} // namespace expr

#endif
