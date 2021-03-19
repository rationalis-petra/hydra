#ifndef __HYDRA_EXPRESSIONS_DATA_BOOL_HPP
#define __HYDRA_EXPRESSIONS_DATA_BOOL_HPP

#include <string>

#include "expressions/object.hpp"
#include "parent.hpp"

namespace expr {
struct t : public Object {
  virtual void mark_node();

  std::string to_string(LocalRuntime &r, LexicalScope& s);
  static t *get();

private:
  static t *singleton;
  t();
};

struct nil : public Object {
  virtual void mark_node();

  std::string to_string(LocalRuntime &r, LexicalScope& s);
  static nil *get();
  bool null() const;

private:
  static nil *singleton;
  nil();
};

} // namespace expr

#endif
