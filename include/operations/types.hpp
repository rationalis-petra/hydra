#ifndef __HYDRA_OP_TYPES_HPP
#define __HYDRA_OP_TYEPS_HPP

#include "expressions.hpp"

struct op_typep : hydra_oper {
  op_typep();
  hydra_object *call(hydra_object *arg_list, runtime &r, lexical_scope &s);
};

#endif
