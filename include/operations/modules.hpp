#ifndef __HYDRA_OPERATIONS_MODULE_HPP
#define __HYDRA_OPERATIONS_MODULE_HPP

#include "expressions.hpp"

struct op_make_symbol : hydra_oper {
  op_make_symbol();
  hydra_object *call(hydra_object *arg_list, runtime &r, lexical_scope &s);
};
struct op_make_module : hydra_oper {
  op_make_module();
  hydra_object *call(hydra_object *arg_list, runtime &r, lexical_scope &s);
};
struct op_in_module : hydra_oper {
  op_in_module();
  hydra_object *call(hydra_object *arg_list, runtime &r, lexical_scope &s);
};
struct op_insert : hydra_oper {
  op_insert();
  hydra_object *call(hydra_object *arg_list, runtime &r, lexical_scope &s);
};
struct op_intern : hydra_oper {
  op_intern();
  hydra_object *call(hydra_object *arg_list, runtime &r, lexical_scope &s);
};
struct op_get_module : hydra_oper {
  op_get_module();
  hydra_object *call(hydra_object *arg_list, runtime &r, lexical_scope &s);
};
struct op_get_symbols : hydra_oper {
  op_get_symbols();
  hydra_object *call(hydra_object *arg_list, runtime &r, lexical_scope &s);
};
struct op_export : hydra_oper {
  op_export();
  hydra_object *call(hydra_object *arg_list, runtime &r, lexical_scope &s);
};
struct op_remove : hydra_oper {
  op_remove();
  hydra_object *call(hydra_object *arg_list, runtime &r, lexical_scope &s);
};
struct op_get : hydra_oper {
  op_get();
  hydra_object *call(hydra_object *arg_list, runtime &r, lexical_scope &s);
};

#endif // __HYDRA_MODULE_HPP
