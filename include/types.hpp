#ifndef __HYDRA_TYPES_HPP
#define __HYDRA_TYPES_HPP

#include <list>

#include "expressions/object.hpp"
#include "types/type.hpp"
#include "types/fn.hpp"

enum foreign_type { Int32, Pointer, String, Void };

hydra_type* type_from_rep(hydra_object* type);

struct type_nil : public hydra_type {
  virtual void mark_node();
  std::string to_string() const;
  virtual hydra_object* check_type(hydra_object* obj);};

struct type_integer : public hydra_type {
  virtual void mark_node();
  std::string to_string() const;
  hydra_object* check_type(hydra_object* obj);
};

struct type_string : public hydra_type {
  virtual void mark_node();
  std::string to_string() const;
  hydra_object* check_type(hydra_object* obj);
};

struct type_symbol : public hydra_type {
  virtual void mark_node();
  std::string to_string() const;
  hydra_object* check_type(hydra_object* obj);
};

struct type_cons : public hydra_type {
  virtual void mark_node();
  std::string to_string() const;
  hydra_object* check_type(hydra_object* obj);
};

struct type_array : public hydra_type {
  virtual void mark_node();
  std::string to_string() const;
  hydra_object* check_type(hydra_object* obj);
};

struct type_list : public hydra_type {
  virtual void mark_node();
  std::string to_string() const;
  hydra_object* check_type(hydra_object* obj);
};

struct type_module : public hydra_type {
  virtual void mark_node();
  std::string to_string() const;
  hydra_object* check_type(hydra_object* obj);
};

struct type_char : public hydra_type {
  virtual void mark_node();
  std::string to_string() const;
  hydra_object* check_type(hydra_object* obj);
};

struct type_stream : public hydra_type {
  virtual void mark_node();
  std::string to_string() const;
  hydra_object* check_type(hydra_object* obj);
};

struct type_type : public hydra_type {
  virtual void mark_node();
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
