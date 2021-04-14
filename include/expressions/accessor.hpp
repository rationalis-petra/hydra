#ifndef __HYDRA_EXPRESSIONS_ACCESSOR_HPP
#define __HYDRA_EXPRESSIONS_ACCESSOR_HPP
#include <list>
#include <map>
#include <set>
#include <string>

#include "operation.hpp"

namespace expr {

struct Reader : public Operator {
  Reader(Object *obj, Symbol *slot);
  Symbol *slot;
  Object *call(std::list<Object *> arg_list, interp::LocalRuntime &r,
               interp::LexicalScope &s, bool);
};
struct Writer : public Operator {
  Writer(Object *obj, Symbol *slot);
  Symbol *slot;
  Object *call(std::list<Object *> arg_list, interp::LocalRuntime &r,
               interp::LexicalScope &s, bool);
};
} // namespace expr

#endif // __VALUE_HPP
