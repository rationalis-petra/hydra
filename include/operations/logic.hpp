#ifndef __HYDRA_OPERATIONS_LOGIC_HPP
#define __HYDRA_OPERATIONS_LOGIC_HPP

#include "expressions.hpp"

namespace op {

  void initialize_logic();

  extern expr::Operator* do_and;
  extern expr::Operator* do_or;
  extern expr::Operator* do_not;

  extern expr::GenericFn *bin_greater;
  extern expr::GenericFn *bin_equal;
  extern expr::GenericFn *greater;
  extern expr::GenericFn *equal;

} // namespace op

#endif // __LOGIC_HPP
