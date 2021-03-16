#ifndef __HYDRA_EXPRESSIONS_DATA_UNION_HPP
#define __HYDRA_EXPRESSIONS_DATA_UNION_HPP

#include <string>
#include <istream>
#include <list>
#include <vector>

#include "expressions/object.hpp"
#include "parent.hpp"

namespace expr {

struct Symbol;

struct Union : public Object {
  virtual void mark_node();
  static Parent* parent;

  std::string to_string() const;
  Symbol *tag;
  Object *value;
};

} // namespace expr

#endif
