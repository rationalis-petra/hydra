#include <list>

#include "expressions.hpp"
#include "operations.hpp"

using std::list;
using std::string;

op_describe::op_describe() {
  is_fn = true;
  docstring = new hydra_string("Describes the provided argument");
  type->arg_list.push_front(new type_nil);
};


hydra_object* op_describe::call(hydra_object* alist, runtime &r, lexical_scope& s) {
  list<hydra_object*> arg_list = get_arg_list(alist, r, s);
  if (arg_list.size() != 1) {
    string err = "incorrect args count for describe";
    throw err;
  }
  if (hydra_oper* op = dynamic_cast<hydra_oper*>(arg_list.front())) {
    string str = "Function:\n" + op->docstring->value +
                 "\nLambda-List: " +
      "\nType: " + op->type->to_string();
    if (user_oper* uop = dynamic_cast<user_oper*>(op)) {
      //str += "\n Expression:\n";
      //str += "(fn " + uop->expr->to_string() + ")";
    }
    return new hydra_string(str);
  } else {
    return new hydra_string("some value...");
  }
}
