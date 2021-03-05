#ifndef __HYDRA_EXPRESSIONS_OBJECT_OBEJCT_HPP
#define __HYDRA_EXPRESSIONS_OBJECT_OBEJCT_HPP

#include <string>

#include "expressions/language.hpp"
#include "object.hpp"


struct hydra_object_object : public hydra_object {
  void mark_node();
  std::string to_string() const;
  std::map<hydra_symbol*, hydra_object*> object_vals;

  std::set<hydra_object_object*> prototypes;
  std::set<hydra_object_object*> derivatives;
};

#endif
// operations: set 
// operation: get


