#ifndef __HYDRA_EXPRESSIONS_HPP
#define __HYDRA_EXPRESSIONS_HPP
// TYPES defines the hydra_object, which is the internal representation of a
// compiled ivy object: the primary purpose of it is 

#include <string>
#include <map>

#include "ltdl.h"


#include "expressions/language.hpp"
#include "expressions/data.hpp"
#include "expressions/foreign.hpp"

struct runtime {
  hydra_module* root;
  hydra_module* active_module;
  std::map<char, hydra_oper*> readtable;
};

struct lexical_scope {
  std::map<hydra_symbol*, hydra_object*> map;
  lexical_scope();
  ~lexical_scope();
};

extern hydra_module* language_module;

#endif // __HYDRA_EXPRESSIONS_HPP
