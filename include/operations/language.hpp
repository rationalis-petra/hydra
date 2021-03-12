#ifndef __HYDRA_OPERATIONS_LANGUAGE_HPP
#define __HYDRA_OPERATIONS_LANGUAGE_HPP

#include "expressions.hpp"

namespace op {
// language
  extern expr::Operator* do_if;
  extern expr::Operator* do_while;
  extern expr::Operator* bind;
  extern expr::Operator* unbind;
  extern expr::Operator* addfn;
  extern expr::Operator* genfn;
  extern expr::Operator* fn;
  extern expr::Operator* mac;
  extern expr::Operator* progn;
  extern expr::Operator* definedp;

  extern expr::Operator* var;
  extern expr::Operator* ref;

  extern expr::Operator* quote;
  extern expr::Operator* read;
  extern expr::Operator* eval;
  extern expr::Operator* set_mac_char;
  extern expr::Operator* exit;

} // namespace op
#endif // __LANGUAGE_HPP
