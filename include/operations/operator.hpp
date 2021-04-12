#ifndef __HYDRA_OPERATIONS_OPERATOR_HPP
#define __HYDRA_OPERATIONS_OPERATOR_HPP

#include "expressions.hpp"


namespace op {
  void initialize_mkoperator();

  extern expr::Operator* addfn;
  extern expr::Operator* genfn;
  extern expr::Operator* fn;
  extern expr::Operator* mac;

}

#endif
