#ifndef __HYDRA_EXPRESSIONS_OBJECT_OBEJCT_HPP
#define __HYDRA_EXPRESSIONS_OBJECT_OBEJCT_HPP

#include <string>

#include "expressions/language.hpp"
#include "object.hpp"


namespace expr {

struct UserObject : public Object {
  void mark_node();
  std::string to_string() const;
  std::map<Symbol*, Object*> object_vals;

  std::set<UserObject*> prototypes;
  std::set<UserObject*> derivatives;
};

}
#endif
// operations: set 
// operation: get


