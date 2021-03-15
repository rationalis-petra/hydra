#include <list>
#include <string>

#include "expressions.hpp"
#include "operations.hpp"

using std::list;
using std::string;

using namespace expr;
using type::hydra_cast;


Object *op_plus(list<Object*> arg_list, LocalRuntime &r, LexicalScope &s) {
  // ASSUME: all values in arg_list are rooted
  Integer *out = new Integer(0);

  for (Object *o : arg_list) {
    Object::roots.remove(o);
    Integer *num = hydra_cast<Integer>(o);
    out->value += num->value;
  }
  return out;
}
expr::Operator *op::plus =
    new InbuiltOperator("Returns the sum of its arguemnts. When "
                        "provided with no arguments, returns 0",
                        op_plus, type::Fn::with_rest(new type::Integer), true);









Object *op_minus(list<Object*> arg_list, LocalRuntime &r, LexicalScope &s) {
  
  if (arg_list.size() < 2) {
    throw "Insufficient arguments provided to '-' function";
  }


  Integer *out = new Integer(0);
  out->value = hydra_cast<Integer>(arg_list.front())->value;
  Object::roots.remove(arg_list.front());

  arg_list.pop_front();

  for (Object *arg : arg_list) {
    Integer *num = hydra_cast<Integer>(arg);
    out->value -= num->value;
  Object::roots.remove(arg);
  }
  return out;
}

expr::Operator *op::minus =
    new InbuiltOperator("Subtract the second and all subsequent"
                        "arguments from the first",
                        op_minus, type::Fn::with_rest(new type::Integer), true);








Object *op_multiply(list<Object*> arg_list, LocalRuntime &r, LexicalScope &s) {
  
  Integer *out = new Integer(1);

  for (Object *arg : arg_list) {
    if (Integer *num = dynamic_cast<Integer *>(arg))
      out->value *= num->value;
  }
  return out;
}

expr::Operator *op::multiply =
    new InbuiltOperator("Returns the product of all its arguments,\n or "
                        "one if no arguments provided",
                        op_multiply, type::Fn::with_rest(new type::Integer), true);











Object *op_divide(list<Object*> arg_list, LocalRuntime &r,
                              LexicalScope &s) {
  
  if (arg_list.size() < 1) {
    throw "Insufficient arguments provided to '/' function";
  }
  Integer *out = new Integer(0);

  out->value = dynamic_cast<Integer *>(arg_list.front())->value;
  arg_list.pop_front();

  for (Object *arg : arg_list) {
    Integer *num = dynamic_cast<Integer *>(arg);
    out->value /= num->value;
  }
  return out;
}

expr::Operator *op::divide = new InbuiltOperator(
    "Divides the first argument by the second and all subsequent arguments",
    op_divide, type::Fn::with_rest(new type::Integer), true);












Object *op_int_gr(list<Object*> arg_list, LocalRuntime &r, LexicalScope &s) {
  
  // we now ASSERT that arg_list is a list of length 2
  // does it contain integers?

  Integer *a1 = dynamic_cast<Integer *>(arg_list.front());
  Integer *a2 = dynamic_cast<Integer *>(arg_list.back());
  if (a1->value >= a2->value) {
    return t::get();
  } else {
    return nil::get();
  }
}

expr::Operator *op::int_gr =
    new InbuiltOperator("Returns t iff the first argument is greater "
                        "than the second, and nil otherwise",
                        op_int_gr, type::Fn::with_rest(new type::Integer), true);


Object *op_int_eq(list<Object*> arg_list, LocalRuntime &r, LexicalScope &s) {
  
  // we now ASSERT that arg_list is a list of length 2
  // does it contain integers?

  Integer *a1 = dynamic_cast<Integer *>(arg_list.front());
  Integer *a2 = dynamic_cast<Integer *>(arg_list.back());
  // so arg_list is a list containing integers!
  if (a1->value == a2->value) {
    return t::get();
  } else {
    return nil::get();
  }
}

expr::Operator *op::int_eq =
  new InbuiltOperator("Equality for Integers",
                      op_int_eq, type::Fn::with_args({new type::Integer, new type::Integer}),
                      true);
