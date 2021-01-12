#ifndef __HYDRA_LOGIC_HPP
#define __HYDRA_LOGIC_HPP

#include "expressions.hpp"

struct op_eq : hydra_oper {
  op_eq();
  hydra_object *call(hydra_object *arg_list, runtime &r);
};
struct op_and : hydra_oper {
  op_and();
  hydra_object *call(hydra_object *arg_list, runtime &r);
};
struct op_or : hydra_oper {
  op_or();
  hydra_object *call(hydra_object *arg_list, runtime &r);
};
struct op_not : hydra_oper {
  op_not();
  hydra_object *call(hydra_object *arg_list, runtime &r);
};

#endif // __LOGIC_HPP
