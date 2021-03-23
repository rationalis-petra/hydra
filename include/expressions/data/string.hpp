#ifndef __HYDRA_EXPRESSIONS_DATA_STRING_HPP
#define __HYDRA_EXPRESSIONS_DATA_STRING_HPP

#include <string>
#include <istream>
#include <list>
#include <vector>

#include "expressions/object.hpp"
#include "expressions/parent.hpp"

namespace expr {

struct HString : public Object {
  virtual void mark_node();
  static Parent* parent;

  HString();
  HString(std::string str);

  std::string to_string(LocalRuntime &r, LexicalScope& s);
  std::string value;  
};

}

#endif
