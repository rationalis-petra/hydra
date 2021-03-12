#ifndef __HYDRA_OPERATIONS_LOGIC_HPP
#define __HYDRA_OPERATIONS_LOGIC_HPP

#include "expressions.hpp"

namespace op {

  extern expr::Operator* eq;
  extern expr::Operator* do_and;
  extern expr::Operator* do_or;
  extern expr::Operator* do_not;

} // namespace op

#endif // __LOGIC_HPP
