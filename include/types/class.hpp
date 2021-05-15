
#ifndef __HYDRA_TYPES_CLASS_HPP
#define __HYDRA_TYPES_CLASS_HPP
#include <list>
#include <map>
#include <set>
#include <string>

#include "fn.hpp"
#include "expressions/object.hpp"
#include "interpreter/garbage.hpp"
#include "interpreter/runtime.hpp"

namespace type {

struct Operator;
struct Parent;

struct TypeClass : public Type {
  TypeClass();
  expr::Object *check_type(expr::Object *obj);
  expr::Object *subtype(Type *obj);
  expr::Object *equal(Type *obj);
  std::string to_string(interp::LocalRuntime& r, interp::LexicalScope& s);
  void mark_node();

  std::vector<Fn*> ops;
};

} // namespace expr

#endif // __VALUE_HPP
