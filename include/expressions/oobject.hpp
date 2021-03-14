#ifndef __HYDRA_EXPRESSIONS_OBJECT_OBEJCT_HPP
#define __HYDRA_EXPRESSIONS_OBJECT_OBEJCT_HPP

#include <string>

#include "expressions/language.hpp"
#include "object.hpp"


namespace expr {

struct UserObject : public Object {
  void mark_node();
  std::string to_string() const;
};

}
#endif
// operations: set 
// operation: get


