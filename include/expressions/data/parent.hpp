#ifndef __HYDRA_EXPRESSIONS_DATA_PARENT_HPP
#define __HYDRA_EXPRESSIONS_DATA_PARENT_HPP

#include <string>

#include "expressions/object.hpp"

namespace expr {

  // Used to represent a parent-class for a datatype, e.g. Integer,
  // Char, etc.
struct Parent : public Object {
  Parent(std::string);
  std::string to_string() const;
  std::string name;
};

}

#endif
