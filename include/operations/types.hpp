#ifndef __HYDRA_OPERATIONS_TYPES_HPP
#define __HYDRA_OPERATIONS_TYPES_HPP

#include "expressions.hpp"

struct op_typep : hydra_oper {
  op_typep();
  hydra_object *call(hydra_object *arg_list, runtime &r, lexical_scope &s);
};

struct op_type : hydra_oper {
  op_type();
  hydra_object *call(hydra_object *arg_list, runtime &r, lexical_scope &s);
};

#endif
