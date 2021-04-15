#ifndef __HYDRA_OPERATIONS_ARITHMETIC_HPP
#define __HYDRA_OPERATIONS_ARITHMETIC_HPP

#include "expressions.hpp"

namespace op {

  extern void mk_arithmetic();
  extern expr::GenericFn *bin_plus;
  extern expr::GenericFn *bin_minus;
  extern expr::GenericFn *bin_multiply;
  extern expr::GenericFn *bin_divide;
  extern expr::GenericFn *bin_greater;
  extern expr::GenericFn *bin_equal;

  extern expr::GenericFn *plus;
  extern expr::GenericFn *minus;
  extern expr::GenericFn *multiply;
  extern expr::GenericFn *divide;
  extern expr::GenericFn *greater;
  extern expr::GenericFn *equal;

} // namespace op

#endif // __HYDRA_ARITHMETIC_HPP
