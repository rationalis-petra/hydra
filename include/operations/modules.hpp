#ifndef __HYDRA_OPERATIONS_MODULE_HPP
#define __HYDRA_OPERATIONS_MODULE_HPP

#include "expressions.hpp"
#include "data.hpp"

namespace op {

  void initialize_module();

  extern expr::Operator* mk_symbol;
  extern expr::Operator* mk_module;
  extern expr::Operator* in_module;
  extern expr::Operator* insert;
  extern expr::Operator* intern;
  extern expr::Operator* get_module;
  extern expr::Operator* get_symbols;
  extern expr::Operator* do_export;
  extern expr::Operator* remove;

} // namespace op
#endif // __HYDRA_MODULE_HPP
