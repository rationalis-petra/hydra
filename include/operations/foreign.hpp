#ifndef __HYDRA_FFI_HPP
#define __HYDRA_FFI_HPP

#include "ffi.h"

#include "expressions.hpp"


// define-foreign-library
struct op_foreign_lib : hydra_oper {
  op_foreign_lib();
  hydra_object *call(hydra_object *body, runtime &r);
};

struct op_foreign_sym : hydra_oper {
  op_foreign_sym();
  hydra_object *call(hydra_object *body, runtime &r);
};

// take a foreign symbol, give it a type & return a hydra object
// that can be used normally
struct op_internalize : hydra_oper {
  op_internalize();
  hydra_object *call(hydra_object *body, runtime &r);
};

// get foreign value
struct op_foreign_get : hydra_oper {
  op_foreign_get();
  hydra_object *call(hydra_object *body, runtime &r);
};

#endif // __HYDRA_FFI_HPP
