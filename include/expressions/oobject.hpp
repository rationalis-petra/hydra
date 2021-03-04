#ifndef __HYDRA_EXPRESSIONS_OBJECT_OBEJCT_HPP
#define __HYDRA_EXPRESSIONS_OBJECT_OBJECT_HPP

#include <string>

#include "expressions/language.hpp"
#include "object.hpp"


struct hydra_object_object : public hydra_object {
  void mark_node();
  std::string to_string() const;
  std::map<hydra_symbol*, hydra_object*> object_vals;
  hydra_object_object* prototype;
};

#endif
// operations: set 
// operation: get


