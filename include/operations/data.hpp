#ifndef __HYDRA_OPERATIONS_DATA_HPP
#define __HYDRA_OPERATIONS_DATA_HPP

#include "expressions.hpp"

struct op_cons : hydra_oper {
  op_cons();
  hydra_object* call(hydra_object* alist, runtime &r, lexical_scope& s);
};
struct op_car : hydra_oper {
  op_car();
  hydra_object* call(hydra_object* alist, runtime &r, lexical_scope& s);
};
struct op_cdr : hydra_oper {
  op_cdr();
  hydra_object* call(hydra_object* alist, runtime &r, lexical_scope& s);
};
struct op_arr : hydra_oper {
  op_arr();
  hydra_object* call(hydra_object* alist, runtime &r, lexical_scope& s);
};
struct op_elt : hydra_oper {
  op_elt();
  hydra_object* call(hydra_object* alist, runtime &r, lexical_scope& s);
};
struct op_lock : hydra_oper {
  op_lock();
  hydra_object* call(hydra_object* alist, runtime &r, lexical_scope& s);
};
struct op_unlock : hydra_oper {
  op_unlock();
  hydra_object* call(hydra_object* alist, runtime &r, lexical_scope& s);
};

#endif // __HYDRA_DATA_HPP
