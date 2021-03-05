#ifndef __HYDRA_OPERATIONS_DEV_HPP
#define __HYDRA_OPERATIONS_DEV_HPP

#include "expressions.hpp"

struct op_describe : hydra_oper {
  op_describe();
  hydra_object* call(hydra_object* alist, runtime &r, lexical_scope& s);
};

struct op_trace : hydra_oper {
  op_trace();
  hydra_object* call(hydra_object* alist, runtime &r, lexical_scope& s);
};

struct op_time : hydra_oper {
  op_time();
  hydra_object* call(hydra_object* alist, runtime &r, lexical_scope& s);
};

#endif // __HYDRA_DATA_HPP
