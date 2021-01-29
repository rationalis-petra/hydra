#ifndef __HYDRA_DEV_HPP
#define __HYDRA_DEV_HPP

#include "expressions.hpp"

struct op_describe : hydra_oper {
  op_describe();
  hydra_object* call(hydra_object* alist, runtime &r, lexical_scope& s);
};

#endif // __HYDRA_DATA_HPP
