#ifndef __HYDRA_OPERATIONS_DEV_HPP
#define __HYDRA_OPERATIONS_DEV_HPP

#include "expressions.hpp"

namespace op {

  void initialize_dev();

  extern expr::Operator *macexpand;
  extern expr::Operator *describe;
  extern expr::Operator *trace;
  extern expr::Operator *time;

} // namespace op

#endif // __HYDRA_DATA_HPP
