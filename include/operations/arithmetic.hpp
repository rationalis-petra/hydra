#ifndef __HYDRA_ARITHMETIC_HPP
#define __HYDRA_ARITHMETIC_HPP

#include "expressions.hpp"

struct op_plus : hydra_oper {
  op_plus();
  hydra_object *call(hydra_object *arg_list, runtime &r, lexical_scope& s);
};
struct op_minus : hydra_oper {
  op_minus();
  hydra_object *call(hydra_object *arg_list, runtime &r, lexical_scope& s);
};
struct op_multiply : hydra_oper {
  op_multiply();
  hydra_object *call(hydra_object *arg_list, runtime &r, lexical_scope& s);
};
struct op_divide : hydra_oper {
  op_divide();
  hydra_object *call(hydra_object *arg_list, runtime &r, lexical_scope& s);
};
struct op_gr : hydra_oper {
  op_gr();
  hydra_object *call(hydra_object *arg_list, runtime &r, lexical_scope& s);
};

#endif // __HYDRA_ARITHMETIC_HPP
