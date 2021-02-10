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

  if (user_oper* uop = dynamic_cast<user_oper*>(arg_list.front())) {
    string str = "Generic Function:\n" + uop->docstring->value +
                 "\nLambda-List: " +
      "\nType: " + uop->type->to_string();
      //str += "\n Expression:\n";
      //str += "(fn " + uop->expr->to_string() + ")";
    return new hydra_string(str);
  } else if (combined_fn* generic = dynamic_cast<combined_fn*>(arg_list.front())) {
    string str = "Generic Function:\n" + generic->docstring->value +
                 "\nLambda-List: " +
      "\nType: " + generic->type->to_string();
    return new hydra_string(str);
  } if (hydra_oper* op = dynamic_cast<hydra_oper*>(arg_list.front())) {
    string str = "Function:\n" + op->docstring->value +
                 "\nLambda-List: " +
      "\nType: " + op->type->to_string();
    return new hydra_string(str);
  } else {
    return new hydra_string("no documentation for this type");
  }
}
