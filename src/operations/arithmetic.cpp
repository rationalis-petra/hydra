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
  out->value = get_inbuilt<Integer *>(arg_list.front())->value;
  Object::roots.remove(arg_list.front());
  arg_list.pop_front();

  for (Object *arg : arg_list) {
    Integer *num = get_inbuilt<Integer *>(arg);
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
  if (a1->value > a2->value) {
    return t::get();
  } else {
    return nil::get();
  }
}

#include <iostream>
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

GenericFn *op::plus;
GenericFn *op::minus;
GenericFn *op::multiply;
GenericFn *op::divide;
GenericFn *op::greater;
GenericFn *op::equal;

void op::mk_arithmetic() {

  Operator *in_op_plus = new InbuiltOperator(
      "Integer +",
      "Returns the sum of its arguemnts. When "
      "provided with no arguments, returns 0",
      op_plus, type::Fn::with_rest(type::integer_type), true);

  op::plus = new GenericFn();
  op::plus->add(in_op_plus);
  plus->type->rest_type = new type::Any;

  Operator *in_op_minus = new InbuiltOperator(
      "Integer -",
      "Subtract the second and all subsequent"
      "arguments from the first",
      op_minus,
      type::Fn::with_all({type::integer_type, type::integer_type},
                         type::integer_type, type::integer_type),
      true);
  op::minus = new GenericFn;
  op::minus->type->rest_type = new type::Any;
  op::minus->add(in_op_minus);

  Operator *in_op_multiply = new InbuiltOperator(
      "Integer *",
      "Returns the product of all its arguments,\n or "
      "one if no arguments provided",
      op_multiply, type::Fn::with_rest(new type::Derives(Integer::parent)),
      true);
  op::multiply = new GenericFn;
  op::multiply->type->rest_type = new type::Any;
  op::multiply->add(in_op_multiply);

  Operator *in_op_divide = new InbuiltOperator(
      "Integer /",
      "Divides the first argument by the second and all subsequent arguments",
      op_divide,
      type::Fn::with_all({type::integer_type, type::integer_type},
                         type::integer_type, type::integer_type),
      true);
  op::divide = new GenericFn;
  op::divide->type->rest_type = new type::Any;
  op::divide->add(in_op_divide);

  Operator *in_op_int_gr = new InbuiltOperator(
      "Integer >",
      "Returns t iff the first argument is greater "
      "than the second, and nil otherwise",
      op_int_gr, type::Fn::with_args({type::integer_type, type::integer_type}),
      true);
  op::greater = new GenericFn;
  op::greater->type->arg_list.push_back(new type::Any);
  op::greater->type->arg_list.push_back(new type::Any);
  op::greater->add(in_op_int_gr);

  Operator *in_op_int_eq = new InbuiltOperator(
      "Integer =", "Equality for Integers", op_int_eq,
      type::Fn::with_args({type::integer_type, type::integer_type}), true);

  op::equal = new GenericFn;
  op::equal->type->arg_list.push_back(new type::Any);
  op::equal->type->arg_list.push_back(new type::Any);
  op::equal->add(in_op_int_eq);
}
