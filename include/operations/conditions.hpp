#ifndef __HYDRA_OPERATIONS_CONDITIONS_HPP
#define __HYDRA_OPERATIONS_CONDITIONS_HPP

#include "expressions.hpp"

namespace op {
  void initialize_conditions();

  extern expr::Operator* handler_bind;
  extern expr::Operator* handler_catch;
  extern expr::Operator* add_restart;
  extern expr::Operator* get_restarts;
  extern expr::Operator* signal_condition;

} // namespace op
#endif
