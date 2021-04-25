#ifndef __HYDRA_EXPRESSIONS_BYTE_STRUCT_HPP
#define __HYDRA_EXPRESSIONS_BYTE_STRUCT_HPP

#include <climits>
#include <string>

#include "expressions/object.hpp"

namespace expr {

  // Used to represent a parent-class for a datatype, e.g. Integer,
  // Char, etc.
struct Byte : public Object {
  Byte();
  std::string to_string(interp::LocalRuntime &r, interp::LexicalScope& s);

  // the byte data
  unsigned char* byte;

};

}

#endif
