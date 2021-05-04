#include <list>

#include "expressions.hpp"
#include "operations.hpp"
#include "utils.hpp"

using std::list;
using std::string;

using namespace expr;
using namespace interp;


Object *op_mk_const(list<Object*> arg_list, LocalRuntime &r, LexicalScope &s) {
  get_inbuilt<Symbol*>(arg_list.front())->mut = false;
  return arg_list.front();
}


Object *op_mk_mutable(list<Object*> arg_list, LocalRuntime &r, LexicalScope &s) {
  get_inbuilt<Symbol*>(arg_list.front())->mut = true;
  return arg_list.front();
}


Operator *op::mk_mutable;
Operator *op::mk_const;

void op::initialize_symbol() {
  op::mk_const = new InbuiltOperator(
                                 "mutable",
      "Takes a symbol, and prevents bind from being used to modify its' value",
      op_mk_mutable,
      type::Fn::with_all({type::symbol_type}, nullptr, type::symbol_type), true);

  op::mk_const = new InbuiltOperator(
                                   "const",
      "Takes a symbol, and will allow bind to be used to modify it's value",
      op_mk_const,
      type::Fn::with_all({type::symbol_type}, nullptr, type::symbol_type), true);
}
