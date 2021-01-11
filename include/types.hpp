#ifndef __HYDRA_TYPES_HPP
#define __HYDRA_TYPES_HPP

#include "expressions.hpp"

enum foreign_type {
  Int32,
  Pointer,
  String,
  Void
};

template<typename T> T* hydra_cast(hydra_object* inp) {
  T* obj = dynamic_cast<T*>(inp);
  if (obj == nullptr) {
    std::string err = "Failed typecast";
    throw err;
  }
  return obj;
}

struct hydra_type {
  bool is_type(hydra_object* o);
};

struct hydra_type_num {
};

struct hydra_type_list {
};

struct hydra_type_fn {
  std::list<hydra_object*> typed_arglist(hydra_object* o);
};


struct fn_type_signature {
  
};

#endif //__HYDRA_TYPES_HPP
