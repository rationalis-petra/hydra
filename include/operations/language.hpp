#ifndef __HYDRA_OPERATIONS_LANGUAGE_HPP
#define __HYDRA_OPERATIONS_LANGUAGE_HPP

#include "expressions.hpp"

namespace op {

  void initialize_language();
  void initialize_read();

// language
  extern expr::Operator* do_if;
  extern expr::Operator* do_while;
  extern expr::Operator* bind;
  extern expr::Operator* unbind;
  extern expr::Operator* progn;
  extern expr::Operator* definedp;

  extern expr::Operator* set_invoker;
  extern expr::Operator* get_invoker;

  extern expr::Operator* quote;
  extern expr::Operator* read;
  extern expr::Operator* eval;
  extern expr::Operator* set_mac_char;
  extern expr::Operator* exit;

} // namespace op
#endif // __LANGUAGE_HPP
