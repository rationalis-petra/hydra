#ifndef __HYDRA_EXPRESSIONS_OBJECT_OBEJCT_HPP
#define __HYDRA_EXPRESSIONS_OBJECT_OBEJCT_HPP

#include <string>

#include "expressions/language.hpp"
#include "object.hpp"


namespace expr {

struct Object : public Value {
  void mark_node();
  std::string to_string() const;
  std::map<Symbol*, Value*> object_vals;

  std::set<Object*> prototypes;
  std::set<Object*> derivatives;
};

}
#endif
// operations: set 
// operation: get


