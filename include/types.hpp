#ifndef __HYDRA_TYPES_HPP
#define __HYDRA_TYPES_HPP

#include <list>

#include "expressions/object.hpp"
#include "types/type.hpp"
#include "types/basic.hpp"
#include "types/fn.hpp"


//void fn_typecheck(hydra_type t, hydra_object* o);

namespace type {

  template <typename T> T *hydra_cast(expr::Object *inp) {
  T *obj = dynamic_cast<T *>(inp);
  if (obj == nullptr) {
    std::string err = std::string("Failed typecast, expected ") + typeid(T).name() +
                      " got " + typeid(inp).name();
    throw err;
  }
  return obj;
}

}
#endif //__HYDRA_TYPES_HPP
