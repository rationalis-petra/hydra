#include <list>

#include "expressions.hpp"
#include "operations.hpp"

using std::list;
using std::string;

using namespace expr;
using namespace interp;

using type::hydra_cast;

Object *op_lock(list<Object*> arg_list, LocalRuntime &r, LexicalScope &s) {
  if (arg_list.size() != 1) {
    string err = "Incorrect number of arguments to lock: " +
                 std::to_string(arg_list.size()) + " expected 1";
    throw err;
  }
  hydra_cast<Symbol>(arg_list.front())->mut = false;
  return arg_list.front();
}


Object *op_unlock(list<Object*> arg_list, LocalRuntime &r, LexicalScope &s) {
  if (arg_list.size() != 1) {
    string err = "Incorrect number of arguments to unlock: " +
                 std::to_string(arg_list.size()) + " expected 1";
    throw err;
  }

  hydra_cast<Symbol>(arg_list.front())->mut = true;
  return arg_list.front();
}


Operator* op::lock;
Operator *op::unlock;

void op::initialize_symbol() {
  op::lock = new InbuiltOperator(
                                 "lock",
      "Takes a symbol, and prevents set from being used to modify its' value",
      op_lock,
      type::Fn::with_all({new type::Symbol}, nullptr, new type::Symbol), true);

  op::unlock = new InbuiltOperator(
                                   "unlock",
      "Takes a symbol, and will allow set to be used to modify it's value",
      op_lock,
      type::Fn::with_all({new type::Symbol}, nullptr, new type::Symbol), true);
}
