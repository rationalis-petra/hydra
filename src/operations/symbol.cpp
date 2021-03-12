#include <list>

#include "expressions.hpp"
#include "operations.hpp"

using std::list;
using std::string;

using namespace expr;
using type::hydra_cast;

Value *op_lock(Operator *op, Value *alist, LocalRuntime &r, LexicalScope &s) {
  list<Value *> arg_list = op->get_arg_list(alist, r, s);
  if (arg_list.size() != 1) {
    string err = "Incorrect number of arguments to lock: " +
                 std::to_string(arg_list.size()) + " expected 1";
    throw err;
  }
  hydra_cast<Symbol>(arg_list.front())->mut = false;
  return arg_list.front();
}

Operator *op::lock = new InbuiltOperator(
    "Takes a symbol, and prevents set from being used to modify its' value",
    op_lock, type::Fn::with_all({new type::Symbol}, nullptr, new type::Symbol),
    true);

Value *op_unlock(Operator *op, Value *alist, LocalRuntime &r, LexicalScope &s) {
  list<Value *> arg_list = op->get_arg_list(alist, r, s);
  if (arg_list.size() != 1) {
    string err = "Incorrect number of arguments to unlock: " +
                 std::to_string(arg_list.size()) + " expected 1";
    throw err;
  }

  hydra_cast<Symbol>(arg_list.front())->mut = true;
  return arg_list.front();
}

Operator *op::unlock = new InbuiltOperator(
    "Takes a symbol, and will allow set to be used to modify it's value",
    op_lock, type::Fn::with_all({new type::Symbol}, nullptr, new type::Symbol),
    true);
