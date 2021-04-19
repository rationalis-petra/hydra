#ifndef __HYDRA_OPERATIONS_IO_HPP
#define __HYDRA_OPERATIONS_IO_HPP

#include "expressions.hpp"

namespace op {

  void initialize_io();

  extern expr::GenericFn* write;

  extern expr::GenericFn* close;
  extern expr::GenericFn* next;
  extern expr::GenericFn* peek;
  extern expr::GenericFn* put;

  extern expr::Operator* endp;

  extern expr::Operator* print;
  extern expr::Operator* open_file;


} // namespace op
#endif // __HYDRA_IO_HPP
