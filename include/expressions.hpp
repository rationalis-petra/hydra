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
  hydra_object* resolve_symbol(hydra_symbol* sym);

  std::map<std::string, hydra_module> modules;
  hydra_module* active_module;

  std::map<std::string, hydra_object*> global_store;

  std::map<char, hydra_oper*> readtable;
};



#endif // __HYDRA_EXPRESSIONS_HPP
