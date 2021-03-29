#ifndef __HYDRA_OPERATIONS_DEV_HPP
#define __HYDRA_OPERATIONS_DEV_HPP

#include "expressions.hpp"

namespace op {

  void initialize_dev();

  extern expr::GenericFn *macexpand;
  extern expr::GenericFn *describe;
  extern expr::GenericFn *trace;
  extern expr::GenericFn *time;

} // namespace op

#endif // __HYDRA_DATA_HPP
