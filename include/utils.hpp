#ifndef HYDRA_UTILS_HPP
#define HYDRA_UTILS_HPP

#include <list>

#include <iostream>
#include "expressions/object.hpp"


std::list<expr::Object*> cons_to_list(expr::Object* v);
expr::Object* list_to_cons(std::list<expr::Object*> v);

expr::Symbol* get_keyword(std::string str);

template<typename T> T get_inbuilt(expr::Object* obj) {
  if (T x = dynamic_cast<T>(obj)) {
    // is just ain instance of the inbuilt type...
    return x;
  } else {
    // is a user-defined type which inherits from it

    std::set<expr::Symbol *> parent_names = obj->parents;
    for (expr::Symbol* sym : parent_names) {
      expr::Object* val = obj->slots[sym];
      T obj = get_inbuilt<T>(val);
      if (obj)  {
        return obj;
      }
    }
    return nullptr;
  }
}

std::string hydra_to_string(expr::Object* obj, interp::LocalRuntime &r, interp::LexicalScope &s);

#endif
