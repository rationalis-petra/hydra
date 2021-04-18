#ifndef __HYDRA_OPERATIONS_DATA_HPP
#define __HYDRA_OPERATIONS_DATA_HPP

#include "expressions.hpp"

namespace op {

  void initialize_data();

  void initialize_symbol();
  void initialize_string();
  void initialize_vector();
  void initialize_tuple();
  void initialize_object();
  void initialize_cons();
  void initialize_user_obj();

  extern expr::GenericFn *len;
  extern expr::GenericFn *get;
  extern expr::GenericFn *set;
  extern expr::GenericFn *cat;

// String operations: creation, indexing, length & concatenation
  extern expr::GenericFn* to_str;
  // character

// Cons-Cell creation, Car, Cdr
  extern expr::Operator* mk_cons;
  extern expr::Operator* car;
  extern expr::Operator* cdr;

// Vector operations: creation, indexing, length & concatenation
  extern expr::Operator* mk_vec;
  extern expr::Operator* mk_union;
  extern expr::Operator* mk_tuple;


// Object operations: creation, slot retreival, inheritance (expr::Operator* op,
// prototyping)
  extern expr::GenericFn* mk_obj;
  extern expr::GenericFn* clone;

// Symbol operations: making mutable/unmutable
  extern expr::Operator* mk_mutable;
  extern expr::Operator* mk_const;

} 

#endif
