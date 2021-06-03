#include <list>
#include <string>

#include "expressions.hpp"
#include "operations.hpp"
#include "utils.hpp"

using std::list;
using std::string;

using namespace expr;
using namespace interp;

Object *op_mk_complex(list<Object *> arg_list, LocalRuntime &r, LexicalScope &s) {
  Number* num1 = get_inbuilt<Number*>(arg_list.front());
  arg_list.pop_front();
  Number* num2 = get_inbuilt<Number*>(arg_list.front());
  return new Complex(num1, num2);
}

Object *op_bin_plus(list<Object *> arg_list, LocalRuntime &r, LexicalScope &s) {
  Number* num1 = get_inbuilt<Number*>(arg_list.front());
  arg_list.pop_front();
  Number* num2 = get_inbuilt<Number*>(arg_list.front());
  return num1->add(num2);
}

Object *op_plus(list<Object *> arg_list, LocalRuntime &r, LexicalScope &s) {
  Object *out = arg_list.front();
  arg_list.pop_front();
  for(Object* num : arg_list) {
    out = op::bin_plus->call({out, num}, r, s);
  }
  return out;
}

Object *op_bin_minus(list<Object *> arg_list, LocalRuntime &r, LexicalScope &s) {
  Number* num1 = get_inbuilt<Number*>(arg_list.front());
  arg_list.pop_front();
  Number* num2 = get_inbuilt<Number*>(arg_list.front());
  return num1->minus(num2);
}
Object *op_minus(list<Object *> arg_list, LocalRuntime &r, LexicalScope &s) {
  Object *out = arg_list.front();
  arg_list.pop_front();
  for(Object* num : arg_list) {
    out = op::bin_minus->call({out, num}, r, s);
  }
  return out;
}

Object *op_bin_divide(list<Object *> arg_list, LocalRuntime &r, LexicalScope &s) {
  Number* num1 = get_inbuilt<Number*>(arg_list.front());
  arg_list.pop_front();
  Number* num2 = get_inbuilt<Number*>(arg_list.front());
  return num1->divide(num2);
}
Object *op_divide(list<Object *> arg_list, LocalRuntime &r, LexicalScope &s) {
  Object *out = arg_list.front();
  arg_list.pop_front();
  for(Object* num : arg_list) {
    out = op::bin_divide->call({out, num}, r, s);
  }
  return out;
}

Object *op_bin_multiply(list<Object *> arg_list, LocalRuntime &r, LexicalScope &s) {
  Number* num1 = get_inbuilt<Number*>(arg_list.front());
  arg_list.pop_front();
  Number* num2 = get_inbuilt<Number*>(arg_list.front());
  return num1->multiply(num2);
}
Object *op_multiply(list<Object *> arg_list, LocalRuntime &r, LexicalScope &s) {
  Object *out = arg_list.front();
  arg_list.pop_front();
  for(Object* num : arg_list) {
    out = op::bin_multiply->call({out, num}, r, s);
  }
  return out;
}

Object *op_int_gr(list<Object *> arg_list, LocalRuntime &r, LexicalScope &s) {
  // we now ASSERT that arg_list is a list of length 2

  Integer *a1 = get_inbuilt<Integer *>(arg_list.front());
  Integer *a2 = get_inbuilt<Integer *>(arg_list.back());
  if (a1->compare(a2) > 0) {
    return t::get();
  } else {
    return nil::get();
  }
}

Object *op_sqrt(list<Object *> arg_list, LocalRuntime &r, LexicalScope &s) {
  // we now ASSERT that arg_list is a list of length 2
  Real *num = get_inbuilt<Real *>(arg_list.front());
  // so arg_list is a list containing integers!
  return num->sqrt();
}

Object *op_int_eq(list<Object *> arg_list, LocalRuntime &r, LexicalScope &s) {
  // we now ASSERT that arg_list is a list of length 2

  Integer *a1 = get_inbuilt<Integer *>(arg_list.front());
  Integer *a2 = get_inbuilt<Integer *>(arg_list.back());
  // so arg_list is a list containing integers!
  if (a1->compare(a2) == 0) {
    return t::get();
  } else {
    return nil::get();
  }
}
Object *op_float_eq(list<Object *> arg_list, LocalRuntime &r, LexicalScope &s) {
  // we now ASSERT that arg_list is a list of length 2

  Float *a1 = get_inbuilt<Float *>(arg_list.front());
  Float *a2 = get_inbuilt<Float *>(arg_list.back());
  // so arg_list is a list containing integers!
  if (a1->value == a2->value) {
    return t::get();
  } else {
    return nil::get();
  }
}

Operator *op::mk_cmplx;

GenericFn *op::bin_plus;
GenericFn *op::bin_minus;
GenericFn *op::bin_multiply;
GenericFn *op::bin_divide;

GenericFn *op::plus;
GenericFn *op::minus;
GenericFn *op::multiply;
GenericFn *op::divide;

GenericFn *op::sqrt;

void op::initialize_arithmetic() {
  // First, do the "binary" versions...
  op::mk_cmplx = new InbuiltOperator(
      "Makes a complex number",
      "Creates a complex number from two reals",
      op_mk_complex, type::Fn::with_args({type::real_type, type::real_type}), true);

  Operator *in_bin_plus = new InbuiltOperator(
      "Binary Addition (+ x y)",
      "Returns the sum of its arguemnts",
      op_bin_plus, type::Fn::with_args({type::number_type, type::number_type}), true);
  op::bin_plus = new GenericFn();
  op::bin_plus->add(in_bin_plus);
  op::bin_plus->type = type::Fn::with_args({type::number_type, type::number_type});


  Operator *in_bin_minus = new InbuiltOperator(
      "Binary Subtraction (- x y)",
      "Returns the difference of its arguemnts",
      op_bin_minus, type::Fn::with_args({type::number_type, type::number_type}), true);
  op::bin_minus = new GenericFn();
  op::bin_minus->add(in_bin_minus);
  op::bin_minus->type = type::Fn::with_args({type::number_type, type::number_type});


  Operator *in_bin_multiply = new InbuiltOperator(
      "Binary Multiplication (* x y)",
      "Returns the product of its arguemnts",
      op_bin_multiply, type::Fn::with_args({type::number_type, type::number_type}), true);
  op::bin_multiply = new GenericFn();
  op::bin_multiply->add(in_bin_multiply);
  op::bin_multiply->type = type::Fn::with_args({type::number_type, type::number_type});


  Operator *in_bin_divide = new InbuiltOperator(
      "Binary Divide (/ x y)",
      "Returns the divisor of its arguemnts",
      op_bin_divide, type::Fn::with_args({type::number_type, type::number_type}), true);
  op::bin_divide = new GenericFn();
  op::bin_divide->add(in_bin_divide);
  op::bin_divide->type = type::Fn::with_args({type::number_type, type::number_type});




  // Now, we do the "non-binary" versions...
  Operator *in_op_plus = new InbuiltOperator(
      "+",
      "Returns the sum of its arguemnts. When "
      "provided with no arguments, returns 0",
      op_plus, type::Fn::with_rest(type::number_type), true);

  op::plus = new GenericFn();
  op::plus->add(in_op_plus);
  plus->type->rest_type = type::number_type;

  Operator *in_op_minus = new InbuiltOperator(
      "-",
      "Subtract the second and all subsequent"
      "arguments from the first",
      op_minus,
      type::Fn::with_all({type::number_type, type::number_type},
                         type::number_type, type::number_type),
      true);
  op::minus = new GenericFn;
  op::minus->type->rest_type = type::number_type;
  op::minus->add(in_op_minus);

  Operator *in_op_multiply = new InbuiltOperator(
      "*",
      "Returns the product of all its arguments,\n or "
      "one if no arguments provided",
      op_multiply, type::Fn::with_rest(type::number_type),
      true);
  op::multiply = new GenericFn;
  op::multiply->type->rest_type = type::number_type;
  op::multiply->add(in_op_multiply);

  Operator *in_op_divide = new InbuiltOperator(
      "/",
      "Divides the first argument by the second and all subsequent arguments",
      op_divide,
      type::Fn::with_all({type::number_type, type::number_type},
                         type::number_type, type::number_type),
      true);
  op::divide = new GenericFn;
  op::divide->type->rest_type = type::number_type;
  op::divide->add(in_op_divide);

  Operator *in_op_sqrt = new InbuiltOperator(
      "sqrt",
      "Returns the square-root of its' argument ",
      op_sqrt, type::Fn::with_args({type::real_type}),
      true);
  op::sqrt = new GenericFn;
  op::sqrt->type = type::Fn::with_args({type::real_type});
  op::sqrt->add(in_op_sqrt);

  Operator *in_op_int_gr = new InbuiltOperator(
      "Integer >",
      "Returns t iff the first argument is greater "
      "than the second, and nil otherwise",
      op_int_gr, type::Fn::with_args({type::integer_type, type::integer_type}),
      true);
  op::bin_greater->add(in_op_int_gr);

  Operator *in_op_int_eq = new InbuiltOperator(
      "Integer =", "Equality for Integers", op_int_eq,
      type::Fn::with_args({type::integer_type, type::integer_type}), true);
  Operator *in_op_float_eq = new InbuiltOperator(
      "Integer =", "Equality for Floats", op_float_eq,
      type::Fn::with_args({type::float_type, type::float_type}), true);

  op::bin_equal->add(in_op_int_eq);
  op::bin_equal->add(in_op_float_eq);
}
