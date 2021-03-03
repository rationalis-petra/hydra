#ifndef __HYDRA_EXPRESSIONS_HPP
#define __HYDRA_EXPRESSIONS_HPP
// TYPES defines the hydra_object, which is the internal representation of a
// compiled ivy object: the primary purpose of it is 

#include <string>
#include <map>

#include "ltdl.h"


#include "expressions/operation.hpp"
#include "expressions/language.hpp"
#include "expressions/data.hpp"
#include "expressions/foreign.hpp"
#include "expressions/handler.hpp"


// the 'runtime' contains all global state this includes
// the root module, the current (or active) module,  
// the readtable, and a stack (vector) of condition handlers

struct runtime {
  hydra_module* root;
  hydra_module* active_module;
  std::map<char, hydra_oper*> readtable;
  std::list<condition_handler*> handlers;
  std::list<hydra_restart*> restarts;
};

// A runtime containing thread-local variables
struct local_runtime {
  runtime& r;
  std::list<condition_handler*> handlers;
  std::list<hydra_restart*> restarts;
  hydra_module* active_module;
};

// the lexical scope contians a mapping of symbols to their local values
struct lexical_scope {
  std::map<hydra_symbol*, hydra_object*> map;
  lexical_scope();
  ~lexical_scope();
};

extern hydra_module* language_module;

#endif // __HYDRA_EXPRESSIONS_HPP
