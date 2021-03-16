#ifndef __HYDRA_EXPRESSIONS_DATA_NUMBER_HPP
#define __HYDRA_EXPRESSIONS_DATA_NUMBER_HPP

#include <string>
#include <istream>
#include <list>
#include <vector>

#include "expressions/object.hpp"
#include "parent.hpp"

namespace expr {

struct Integer : public Object {
  static Parent* parent;

  Integer(int num);
  std::string to_string() const;

  int value;
};

}

#endif
