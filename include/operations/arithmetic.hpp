#ifndef __HYDRA_OPERATIONS_ARITHMETIC_HPP
#define __HYDRA_OPERATIONS_ARITHMETIC_HPP

#include "expressions.hpp"

namespace op {

  extern void mk_arithmetic();
  extern expr::Operator *plus;
  extern expr::Operator *minus;
  extern expr::Operator *multiply;
  extern expr::Operator *divide;
  extern expr::Operator *int_gr;
  extern expr::Operator *int_eq;

} // namespace op

#endif // __HYDRA_ARITHMETIC_HPP
