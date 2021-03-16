#include <list>
#include <string>

#include "expressions.hpp"
#include "operations.hpp"
#include "utils.hpp"

using std::list;
using std::string;

using namespace expr;

Object *op_plus(list<Object *> arg_list, LocalRuntime &r, LexicalScope &s) {
  // ASSUME: all values in arg_list are rooted
  Integer *out = new Integer(0);

  for (Object *o : arg_list) {
    Object::roots.remove(o);
    Integer *num = get_inbuilt<Integer *>(o);
    out->value += num->value;
  }
  return out;
}

Object *op_minus(list<Object *> arg_list, LocalRuntime &r, LexicalScope &s) {
  Integer *out = new Integer(0);
  out->value = get_inbuilt<Integer*>(arg_list.front())->value;
  Object::roots.remove(arg_list.front());
  arg_list.pop_front();

  for (Object *arg : arg_list) {
    Integer *num = get_inbuilt<Integer*>(arg);
    out->value -= num->value;
    Object::roots.remove(arg);
  }
  return out;
}

Object *op_divide(list<Object *> arg_list, LocalRuntime &r, LexicalScope &s) {

  Integer *out = new Integer(0);
  out->value = get_inbuilt<Integer *>(arg_list.front())->value;
  arg_list.pop_front();

  for (Object *arg : arg_list) {
    Integer *num = get_inbuilt<Integer *>(arg);
    out->value /= num->value;
  }
  return out;
}

Object *op_multiply(list<Object *> arg_list, LocalRuntime &r, LexicalScope &s) {

  Integer *out = new Integer(1);

  for (Object *arg : arg_list) {
    if (Integer *num = get_inbuilt<Integer *>(arg))
      out->value *= num->value;
  }
  return out;
}

Object *op_int_gr(list<Object *> arg_list, LocalRuntime &r, LexicalScope &s) {
  // we now ASSERT that arg_list is a list of length 2

  Integer *a1 = get_inbuilt<Integer *>(arg_list.front());
  Integer *a2 = get_inbuilt<Integer *>(arg_list.back());
  if (a1->value >= a2->value) {
    return t::get();
  } else {
    return nil::get();
  }
}

Object *op_int_eq(list<Object *> arg_list, LocalRuntime &r, LexicalScope &s) {
  // we now ASSERT that arg_list is a list of length 2

  Integer *a1 = get_inbuilt<Integer *>(arg_list.front());
  Integer *a2 = get_inbuilt<Integer *>(arg_list.back());
  // so arg_list is a list containing integers!
  if (a1->value == a2->value) {
    return t::get();
  } else {
    return nil::get();
  }
}

Operator* op::plus;
Operator* op::minus;
Operator* op::multiply;
Operator* op::divide;
Operator* op::int_gr;
Operator* op::int_eq;

void op::mk_arithmetic() {

  op::plus = new InbuiltOperator(
      "Returns the sum of its arguemnts. When "
      "provided with no arguments, returns 0",
      op_plus, type::Fn::with_rest(type::integer_type), true);

  op::minus = new InbuiltOperator(
      "Subtract the second and all subsequent"
      "arguments from the first",
      op_minus,
      type::Fn::with_all({type::integer_type,
                          type::integer_type},
                          type::integer_type,
                          type::integer_type),
      true);

  op::multiply = new InbuiltOperator(
      "Returns the product of all its arguments,\n or "
      "one if no arguments provided",
      op_multiply, type::Fn::with_rest(new type::Derives(Integer::parent)),
      true);

  op::divide = new InbuiltOperator(
      "Divides the first argument by the second and all subsequent arguments",
      op_divide,
      type::Fn::with_all({type::integer_type,
                          type::integer_type},
                          type::integer_type,
                          type::integer_type),
      true);

  op::int_gr = new InbuiltOperator(
      "Returns t iff the first argument is greater "
      "than the second, and nil otherwise",
      op_int_gr,
      type::Fn::with_args({type::integer_type,
                           type::integer_type}),
      true);

  op::int_eq = new InbuiltOperator(
      "Equality for Integers", op_int_eq,
      type::Fn::with_args({type::integer_type,
                           type::integer_type}),
      true);
}
