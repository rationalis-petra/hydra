#ifndef __HYDRA_OPERATIONS_NETWORK_HPP
#define __HYDRA_OPERATIONS_NETWORK_HPP

#include "expressions.hpp"

namespace op {
  extern expr::Operator* mk_socket;

  void initialize_network();

} // namespace op
#endif // __HYDRA_MODULE_HPP
