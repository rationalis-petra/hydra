#ifndef __HYDRA_OPERATIONS_DATA_HPP
#define __HYDRA_OPERATIONS_DATA_HPP

#include "expressions.hpp"

namespace op {

  void initialize_symbol();

  void initialize_string();
  void initialize_vector();
  void initialize_tuple();
  void initialize_object();
  void initialize_cons();
  void initialize_user_obj();

// Cons-Cell creation, Car, Cdr
  extern expr::Operator* mk_cons;
  extern expr::Operator* car;
  extern expr::Operator* cdr;
  extern expr::Operator* cons_eq;

// Vector operations: creation, indexing, length & concatenation
  extern expr::Operator* mk_vec;
  extern expr::Operator* vec_elt;
  extern expr::Operator* vec_set;
  extern expr::Operator* vec_len;
  extern expr::Operator* vec_cat;
  extern expr::Operator* vec_eq;

  extern expr::Operator* mk_union;

  extern expr::Operator* mk_tuple;
  extern expr::Operator* tuple_elt;
  extern expr::Operator* tuple_eq;

// Stirng operations: creation, indexing, length & concatenation
  extern expr::Operator* to_str;
  extern expr::Operator* str_elt;
  extern expr::Operator* str_gr;
  extern expr::Operator* str_len;
  extern expr::Operator* str_cat;
  extern expr::Operator* str_eq;
  extern expr::Operator* char_eq;

// Stirng operations: creation, slot retreival, inheritance (expr::Operator* op,
// prototyping)
  extern expr::Operator* mk_obj;
  extern expr::Operator* obj_get;
  extern expr::Operator* obj_set;
  extern expr::Operator* clone;
  extern expr::Operator* obj_eq;

// Symbol operations: making mutable/unmutable
  extern expr::Operator* lock;
  extern expr::Operator* unlock;

} 

#endif
