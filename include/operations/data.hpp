#ifndef __HYDRA_OPERATIONS_DATA_HPP
#define __HYDRA_OPERATIONS_DATA_HPP

#include "expressions.hpp"

namespace op {

// Cons-Cell creation, Car, Cdr
  extern expr::Operator* mk_cons;
  extern expr::Operator* car;
  extern expr::Operator* cdr;

// Vector operations: creation, indexing, length & concatenation
  extern expr::Operator* mk_vec;
  extern expr::Operator* vec_elt;
  extern expr::Operator* vec_set;
  extern expr::Operator* vec_len;
  extern expr::Operator* vec_cat;

  extern expr::Operator* mk_union;

  extern expr::Operator* mk_tuple;
  extern expr::Operator* tuple_elt;

// Stirng operations: creation, indexing, length & concatenation
  extern expr::Operator* str_elt;
  extern expr::Operator* str_gr;
  extern expr::Operator* str_len;
  extern expr::Operator* str_cat;

// Stirng operations: creation, slot retreival, inheritance (expr::Operator* op,
// prototyping)
  extern expr::Operator* mk_obj;
  extern expr::Operator* obj_get;
  extern expr::Operator* obj_set;
  extern expr::Operator* clone;

// Symbol operations: making mutable/unmutable
  extern expr::Operator* lock;
  extern expr::Operator* unlock;

} // namespace op
#endif // __HYDRA_DATA_HPP
