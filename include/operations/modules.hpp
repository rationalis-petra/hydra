#ifndef __HYDRA_MODULE_HPP
#define __HYDRA_MODULE_HPP

#include "expressions.hpp"

struct op_make_symbol : hydra_oper {
  op_make_symbol();
  hydra_object *call(hydra_object *arg_list, runtime &r);
};
struct op_make_module : hydra_oper {
  op_make_module();
  hydra_object *call(hydra_object *arg_list, runtime &r);
};
struct op_in_module : hydra_oper {
  op_in_module();
  hydra_object *call(hydra_object *arg_list, runtime &r);
};
struct op_insert : hydra_oper {
  op_insert();
  hydra_object *call(hydra_object *arg_list, runtime &r);
};
struct op_get : hydra_oper {
  op_get();
  hydra_object *call(hydra_object *arg_list, runtime &r);
};

#endif // __HYDRA_MODULE_HPP
