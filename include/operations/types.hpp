#ifndef __HYDRA_OPERATIONS_TYPES_HPP
#define __HYDRA_OPERATIONS_TYPES_HPP

#include "expressions.hpp"

namespace op {

  extern expr::Operator* typep;
  extern expr::Operator* mk_type;
  extern expr::Operator* subtype;
  extern expr::Operator* type_eq;

}
#endif
