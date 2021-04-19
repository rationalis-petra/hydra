#ifndef __HYDRA_OPERATIONS_NETWORK_HPP
#define __HYDRA_OPERATIONS_NETWORK_HPP

#include "expressions.hpp"

namespace op {
  extern expr::Operator* mk_tcp_acceptor;
  extern expr::Operator* mk_tcp_socket;

  extern expr::GenericFn* accept;

  void initialize_network();

} // namespace op
#endif // __HYDRA_MODULE_HPP
