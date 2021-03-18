#ifndef __HYDRA_OPERATIONS_IO_HPP
#define __HYDRA_OPERATIONS_IO_HPP

#include "expressions.hpp"

namespace op {

  void initialize_io();

  extern expr::Operator* print;
  extern expr::Operator* open_file;
  extern expr::Operator* close;
  extern expr::Operator* endp;

  extern expr::Operator* next;
  extern expr::Operator* peek;
  extern expr::Operator* put;

} // namespace op
#endif // __HYDRA_IO_HPP
