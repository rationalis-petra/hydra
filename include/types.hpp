#ifndef __HYDRA_TYPES_HPP
#define __HYDRA_TYPES_HPP

#include <list>

#include "expressions/object.hpp"

enum foreign_type { Int32, Pointer, String, Void };


struct hydra_type : public hydra_object {
public:
  virtual hydra_object* check_type(hydra_object* obj) = 0;
};

hydra_type* type_from_rep(hydra_object* type);

struct type_integer : public hydra_type {
  std::string to_string() const;
  hydra_object* check_type(hydra_object* obj);
};

struct type_string : public hydra_type {
  std::string to_string() const;
  hydra_object* check_type(hydra_object* obj);
};

struct type_symbol : public hydra_type {
  std::string to_string() const;
  hydra_object* check_type(hydra_object* obj);
};

struct type_cons : public hydra_type {
  std::string to_string() const;
  hydra_object* check_type(hydra_object* obj);
};

struct type_list : public hydra_type {
  std::string to_string() const;
  hydra_object* check_type(hydra_object* obj);
};

struct type_module : public hydra_type {
  std::string to_string() const;
  hydra_object* check_type(hydra_object* obj);
};

//void fn_typecheck(hydra_type t, hydra_object* o);

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
