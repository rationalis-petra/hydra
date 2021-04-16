#ifndef __HYDRA_OPERATIONS_ARITHMETIC_HPP
#define __HYDRA_OPERATIONS_ARITHMETIC_HPP

#include "expressions.hpp"

namespace op {

  extern void initialize_arithmetic();
  extern expr::Operator *mk_cmplx;
  extern expr::GenericFn *bin_plus;
  extern expr::GenericFn *bin_minus;
  extern expr::GenericFn *bin_multiply;
  extern expr::GenericFn *bin_divide;

  extern expr::GenericFn *plus;
  extern expr::GenericFn *minus;
  extern expr::GenericFn *multiply;
  extern expr::GenericFn *divide;

  extern expr::GenericFn *sqrt;
} // namespace op

#endif // __HYDRA_ARITHMETIC_HPP
