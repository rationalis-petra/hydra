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
struct op_vec : hydra_oper {
  op_vec();
  hydra_object* call(hydra_object* alist, runtime &r, lexical_scope& s);
};


struct op_union : hydra_oper {
  op_union();
  hydra_object* call(hydra_object* alist, runtime &r, lexical_scope& s);
};
struct op_tuple : hydra_oper {
  op_tuple();
  hydra_object* call(hydra_object* alist, runtime &r, lexical_scope& s);
};
struct op_tuple_elt : hydra_oper {
  op_tuple_elt();
  hydra_object* call(hydra_object* alist, runtime &r, lexical_scope& s);
};

// GET DATA
struct op_vec_elt : hydra_oper {
  op_vec_elt();
  hydra_object* call(hydra_object* alist, runtime &r, lexical_scope& s);
};
struct op_str_elt : hydra_oper {
  op_str_elt();
  hydra_object* call(hydra_object* alist, runtime &r, lexical_scope& s);
};

// Length

struct op_vec_len : hydra_oper {
  op_vec_len();
  hydra_object* call(hydra_object* alist, runtime &r, lexical_scope& s);
};

// COMPARISON
struct op_str_gr : hydra_oper {
  op_str_gr();
  hydra_object* call(hydra_object* alist, runtime &r, lexical_scope& s);
};


// CONCATENATE
struct op_vec_cat : hydra_oper {
  op_vec_cat();
  hydra_object* call(hydra_object* alist, runtime &r, lexical_scope& s);
};
struct op_str_cat : hydra_oper {
  op_str_cat();
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

// OBJECTS
struct op_obj_get : hydra_oper {
  op_obj_get();
  hydra_object* call(hydra_object* alist, runtime &r, lexical_scope& s);
};

struct op_object : hydra_oper {
  op_object();
  hydra_object* call(hydra_object* alist, runtime &r, lexical_scope& s);
};

#endif // __HYDRA_DATA_HPP
