#ifndef __HYDRA_EXPRESSIONS_DATA_TUPLE_HPP
#define __HYDRA_EXPRESSIONS_DATA_TUPLE_HPP

#include <string>
#include <istream>
#include <list>
#include <vector>

#include "expressions/object.hpp"
#include "parent.hpp"

namespace expr {

struct Tuple : public Object {
  static Parent* parent;
  virtual void mark_node();

  // expose vector access
  Object* operator[](int i);
  int size();

  std::string to_string() const;
  std::vector<Object*> values;
};

}

#endif
