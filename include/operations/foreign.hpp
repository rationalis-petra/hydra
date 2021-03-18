#ifndef __HYDRA_OPERATIONS_FFI_HPP
#define __HYDRA_OPERATIONS_FFI_HPP

#include "ffi.h"

#include "expressions.hpp"

// define-foreign-library
namespace op {

  void initialize_foreign();

  // Find a foreign library on disk & return handle
  extern expr::Operator* foreign_lib;
  // Find a foreign symbol in a given library
  extern expr::Operator* foreign_sym;
  // Convert a foreign symbol into a native function
  extern expr::Operator* internalize;
  // get a foreign variable as an internal variable
  extern expr::Operator* foreign_get;

}; // namespace op

#endif // __HYDRA_FFI_HPP
