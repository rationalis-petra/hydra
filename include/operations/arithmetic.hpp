#ifndef __HYDRA_ARITHMETIC_HPP
#define __HYDRA_ARITHMETIC_HPP

#include "expressions.hpp"

struct op_plus : hydra_oper {
  op_plus();
  hydra_object *call(hydra_object *arg_list, runtime &r);
};
struct op_minus : hydra_oper {
  op_minus();
  hydra_object *call(hydra_object *arg_list, runtime &r);
};
struct op_multiply : hydra_oper {
  op_multiply();
  hydra_object *call(hydra_object *arg_list, runtime &r);
};
struct op_divide : hydra_oper {
  op_divide();
  hydra_object *call(hydra_object *arg_list, runtime &r);
};
struct op_geq : hydra_oper {
  op_geq();
  hydra_object *call(hydra_object *arg_list, runtime &r);
};

#endif // __HYDRA_ARITHMETIC_HPP
