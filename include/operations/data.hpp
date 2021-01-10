#ifndef __HYDRA_DATA_HPP
#define __HYDRA_DATA_HPP

#include "expressions.hpp"

struct op_cons : hydra_oper {
  op_cons();
  hydra_object *call(hydra_object *body, runtime &r);
};
struct op_car : hydra_oper {
  op_car();
  hydra_object *call(hydra_object *body, runtime &r);
};
struct op_cdr : hydra_oper {
  op_cdr();
  hydra_object *call(hydra_object *body, runtime &r);
};
struct op_arr : hydra_oper {
  op_arr();
  hydra_object *call(hydra_object *body, runtime &r);
};
struct op_elt : hydra_oper {
  op_elt();
  hydra_object *call(hydra_object *body, runtime &r);
};

#endif // __HYDRA_DATA_HPP
