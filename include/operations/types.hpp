#ifndef __HYDRA_OPERATIONS_TYPES_HPP
#define __HYDRA_OPERATIONS_TYPES_HPP

#include "expressions.hpp"


namespace op {
  void initialize_type_ops();
  void type_type_ops();

  extern expr::Operator* typep;
  extern expr::Operator* mk_type;
  extern expr::Operator* subtype;
  extern expr::Operator* type_eq;

  extern expr::Operator* mk_derives;
  extern expr::Operator* mk_is;
  extern expr::Operator* mk_tuple_type;
  extern expr::Operator* mk_vector_type;
  extern expr::Operator* mk_cons_type;

  extern expr::Operator* mk_mac_type;
  extern expr::Operator* mk_fn_type;
}

#endif
