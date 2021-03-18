#include <list>

#include "expressions.hpp"
#include "operations.hpp"

using std::list;
using std::string;

using namespace expr;

Object *op_or(list<Object *> arg_list, LocalRuntime &r, LexicalScope &s) {

  if (arg_list.size() < 2) {
    string err = "Invalid number of arguments to macro or";
    throw err;
  }

  for (Object *arg : arg_list) {
    Object *cond = arg->eval(r, s);
    if (!cond->null())
      return cond;
  }
  return nil::get();
}

Object *op_and(list<Object *> arg_list, LocalRuntime &r, LexicalScope &s) {

  if (arg_list.size() < 2) {
    string err = "Invalid number of arguments to macro and";
    throw err;
  }

  Object *out = nullptr;
  for (Object *arg : arg_list) {
    Object *cond = arg->eval(r, s);
    if (cond->null()) {
      return nil::get();
    } else {
      out = cond;
    }
  }
  if (!out)
    out = nil::get();
  return out;
}

Object *op_not(list<Object *> arg_list, LocalRuntime &r, LexicalScope &s) {

  if (arg_list.size() != 1) {
    string err = "Invalid number of arguments to macro not";
    throw err;
  }
  if (arg_list.front()->null()) {
    return t::get();
  } else {
    return nil::get();
  }
}

Operator *op::do_or;
Operator *op::do_and;
Operator *op::do_not;

void op::initialize_logic() {
  op::do_or = new InbuiltOperator(
      "Returns the first non-nil argument, or nil if all arguments are nil",
      op_or, type::Fn::with_rest(new type::Any), false);
  op::do_and = new InbuiltOperator(
      "Returns the first nil argument, or the last argument if all are non-nil",
      op_and, type::Fn::with_rest(new type::Any), false);
  op::do_not =
      new InbuiltOperator("Returns t if argument is nil, and nil otherwise",
                          op_not, type::Fn::with_args({new type::Any}), true);
}
