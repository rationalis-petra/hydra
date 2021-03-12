#include <list>
#include <string>

#include "expressions.hpp"
#include "operations.hpp"

using std::list;
using std::string;
using std::to_string;

using namespace expr;
using type::hydra_cast;


Value *op_plus(Operator* op, Value *alist, LocalRuntime &r, LexicalScope &s) {
  // we guarantee that this is a cons object
  std::list<Value *> arg_list = op->get_arg_list(alist, r, s);
  Integer *out = new Integer(0);

  for (Value *o : arg_list) {
    Integer *num = hydra_cast<Integer>(o);
    out->value += num->value;
  }
  return out;
}

Value *op_minus(Operator *op, Value *alist, LocalRuntime &r, LexicalScope &s) {
  list<Value *> arg_list = op->get_arg_list(alist, r, s);
  if (arg_list.size() < 2) {
    throw "Insufficient arguments provided to '-' function";
  }
  Integer *out = new Integer(0);
  out->value = hydra_cast<Integer>(arg_list.front())->value;
  arg_list.pop_front();

  for (Value *arg : arg_list) {
    Integer *num = hydra_cast<Integer>(arg);
    out->value -= num->value;
  }
  return out;
}

Value *op_multiply(Operator *op, Value *alist, LocalRuntime &r, LexicalScope &s) {
  list<Value *> arg_list = op->get_arg_list(alist, r, s);
  Integer *out = new Integer(1);

  for (Value *arg : arg_list) {
    if (Integer *num = dynamic_cast<Integer *>(arg))
      out->value *= num->value;
  }
  return out;
}


Value *op_divide(Operator* op, Value *alist, LocalRuntime &r,
                              LexicalScope &s) {
  list<Value *> arg_list = op->get_arg_list(alist, r, s);
  if (arg_list.size() < 1) {
    throw "Insufficient arguments provided to '/' function";
  }
  Integer *out = new Integer(0);

  out->value = dynamic_cast<Integer *>(arg_list.front())->value;
  arg_list.pop_front();

  for (Value *arg : arg_list) {
    Integer *num = dynamic_cast<Integer *>(arg);
    out->value /= num->value;
  }
  return out;
}

Value *op_int_gr(Operator* op, Value *alist, LocalRuntime &r, LexicalScope &s) {
  list<Value *> arg_list = op->get_arg_list(alist, r, s);
  // we now ASSERT that arg_list is a list of length 2
  // does it contain integers?

  Integer *a1 = dynamic_cast<Integer *>(arg_list.front());
  arg_list.pop_front();
  Integer *a2 = dynamic_cast<Integer *>(arg_list.front());
  // so arg_list is a list containing integers!
  int arg1 = a1->value;
  int arg2 = a2->value;

  if (arg1 >= arg2) {
    return t::get();
  } else {
    return nil::get();
  }
}

expr::Operator *op::plus =
    new InbuiltOperator("Returns the sum of its arguemnts. When "
                        "provided with no arguments, returns 0",
                        op_plus, type::Fn::with_rest(new type::Integer), true);

expr::Operator *op::minus =
    new InbuiltOperator("Subtract the second and all subsequent"
                        "arguments from the first",
                        op_minus, type::Fn::with_rest(new type::Integer), true);

expr::Operator *op::multiply =
    new InbuiltOperator("Returns the product of all its arguments,\n or "
                        "one if no arguments provided",
                        op_multiply, type::Fn::with_rest(new type::Integer), true);

expr::Operator *op::divide = new InbuiltOperator(
    "Divides the first argument by the second and all subsequent arguments",
    op_divide, type::Fn::with_rest(new type::Integer), true);

expr::Operator *op::int_gr =
    new InbuiltOperator("Returns t iff the first argument is greater "
                        "than the second, and nil otherwise",
                        op_int_gr, type::Fn::with_rest(new type::Integer), true);
