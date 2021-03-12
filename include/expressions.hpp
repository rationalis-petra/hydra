#ifndef __HYDRA_EXPRESSIONS_HPP
#define __HYDRA_EXPRESSIONS_HPP
// TYPES defines the hydra_object, which is the internal representation of a
// compiled ivy object: the primary purpose of it is 

#include <string>
#include <map>

#include "ltdl.h"


#include "expressions/operation.hpp"
#include "expressions/oobject.hpp"
#include "expressions/language.hpp"
#include "expressions/data.hpp"
#include "expressions/foreign.hpp"
#include "expressions/handler.hpp"

// the 'runtime' contains all global state this includes
// the root module, the current (or active) module,  
// the readtable, and a stack (vector) of condition handlers

namespace expr {

struct LocalRuntime;

struct Runtime {
  Module* root;
  std::map<char, Operator*> readtable;
  std::vector<LocalRuntime*> local_runtimes;
};

// A runtime containing thread-local variables
struct LocalRuntime {
  LocalRuntime(Runtime& r);
  Runtime& r;
  std::list<condition_handler*> handlers;
  std::list<hydra_restart*> restarts;
  Module* active_module;
};

// the lexical scope contians a mapping of symbols to their local values
struct LexicalScope {
  std::map<Symbol*, Value*> map;
  LexicalScope();
  ~LexicalScope();
};

extern Module* language_module;

}
#endif // __HYDRA_EXPRESSIONS_HPP
