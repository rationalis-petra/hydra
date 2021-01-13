#ifndef __HYDRA_TYPES_HPP
#define __HYDRA_TYPES_HPP

#include <list>

#include "expressions/object.hpp"

enum foreign_type { Int32, Pointer, String, Void };


struct hydra_type {
  enum {
    // simple
    type_int,
    type_char,
    type_symbol,
    type_t,
    type_null,
    type_cons,
    // composite data structures
    type_array,
    type_list,
    type_fn,
    type_mac,
    // foreign stuff
    type_fsym,
    type_flib
  } tag;
  // for composite types (symbols, lists, ...)
  std::list<hydra_type> type_params;
};

bool typep(hydra_type t, hydra_object* o);
void fn_typecheck(hydra_type t, hydra_object* o);

template <typename T> T *hydra_cast(hydra_object *inp) {
  T *obj = dynamic_cast<T *>(inp);
  if (obj == nullptr) {
    std::string err = std::string("Failed typecast, expected ") + typeid(T).name() +
                      " got " + typeid(inp).name();
    throw err;
  }
  return obj;
}

#endif //__HYDRA_TYPES_HPP
