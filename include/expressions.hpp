#ifndef __HYDRA_EXPRESSIONS_HPP
#define __HYDRA_EXPRESSIONS_HPP
// TYPES defines the hydra_object, which is the internal representation of a
// compiled ivy object: the primary purpose of it is 

#include <string>
#include <map>

#include "ltdl.h"


#include "expressions/runtime.hpp"
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

// Here, we provide pointers to important objects in the runtime
extern expr::GenericFn *equal_operator;

extern expr::Module *language_module;
extern expr::Module *keyword_module;
extern expr::Module *core_module;

} // namespace expr

#endif // __HYDRA_EXPRESSIONS_HPP
