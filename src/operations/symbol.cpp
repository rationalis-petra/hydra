#include <list>

#include "expressions.hpp"
#include "operations.hpp"

using std::list;
using std::string;

op_lock::op_lock() {
  is_fn = true;
  docstring = new hydra_string(
      "Takes a symbol, and prevents set from being used to modify its' value");
}

hydra_object *op_lock::call(hydra_object *alist, runtime &r, lexical_scope &s) {
  list<hydra_object *> arg_list = get_arg_list(alist, r, s);
  if (arg_list.size() != 1) {
    string err = "Incorrect number of arguments to lock: " +
                 std::to_string(arg_list.size()) + " expected 1";
    throw err;
  }
  hydra_cast<hydra_symbol>(arg_list.front())->mut = false;
  return arg_list.front();
}





op_unlock::op_unlock() {
  docstring = new hydra_string(
      "Takes a symbol, and will allow set to be used to modify it's value");
}

hydra_object *op_unlock::call(hydra_object *alist, runtime &r,
                              lexical_scope &s) {
  list<hydra_object *> arg_list = get_arg_list(alist, r, s);
  if (arg_list.size() != 1) {
    string err = "Incorrect number of arguments to unlock: " +
                 std::to_string(arg_list.size()) + " expected 1";
    throw err;
  }

  hydra_cast<hydra_symbol>(arg_list.front())->mut = true;
  return arg_list.front();
}
