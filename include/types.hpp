#ifndef __HYDRA_TYPES_HPP
#define __HYDRA_TYPES_HPP

#include "expressions.hpp"

enum foreign_type {
  Int32,
  Pointer,
  String,
  Void
};

void test_glfw_init();
void test_glfw_window();
// struct hydra_type {
//   bool typep(hydra_object* obj);
// };

// struct type_t : public hydra_type {};

// struct type_num : public hydra_type {};

// struct type_istream : public hydra_type {};

// struct type_ostream : public hydra_type {};


#endif //__HYDRA_TYPES_HPP
