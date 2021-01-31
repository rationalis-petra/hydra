#ifndef __HYDRA_TYPE_FN_HPP
#define __HYDRA_TYPE_FN_HPP

#include <string>
#include <list>

#include "expressions.hpp"
#include "operations.hpp"

#include "types/type.hpp"

struct type_fn : public hydra_type {
  std::string to_string() const;
  hydra_object* check_type(hydra_object* obj);

  hydra_type* return_type;
  std::list<hydra_type*> arg_list;
  std::list<hydra_type*> optional_list;
  std::list<hydra_symbol*> keyword_names;
  std::list<hydra_type*> keyword_list;
};

#endif
