#include <list>
#include <string>

#include "expressions.hpp"
#include "operations.hpp"
#include "types.hpp"

using std::list;
using std::string;

struct op_typep : hydra_oper() {};

op_typep::op_typep() { eval_args = true; }
hydra_object *op_typep::call(hydra_object *alist, runtime &r,
                             lexical_scope &s) {
  list<hydra_object *> arg_list = get_arg_list(alist, r, s);
  if (arg_list.size() != 2) {
    string err = "typep expects exactly 2 arguments";
    throw err;
  }
  hydra_object *obj = arg_list.front();
  hydra_object *type_rep = arg_list.back();

  hydra_type *type = type_from_rep(type_rep);

  return type->check_type(obj);
}
