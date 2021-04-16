#include <list>

#include "expressions.hpp"
#include "operations.hpp"

using std::list;
using std::string;

using namespace expr;
using namespace interp;

Object *op_equal(list<Object *> arg_list, LocalRuntime &r, LexicalScope &s) {
  Object* out = t::get();

  Object* comp = arg_list.front();
  arg_list.pop_front();
  for (Object* val : arg_list) {
    if (op::bin_equal->call({comp, val}, r, s)->null()) {
      out = nil::get();
    }
    comp = val;
  }
  return out;
}

Object *op_greater(list<Object *> arg_list, LocalRuntime &r, LexicalScope &s) {
  Object* out = t::get();

  Object* comp = arg_list.front();
  arg_list.pop_front();
  for (Object* val : arg_list) {
    if (op::bin_greater->call({comp, val}, r, s)->null()) {
      out = nil::get();
    }
    comp = val;
  }
  return out;
}


Object *op_or(list<Object *> arg_list, LocalRuntime &r, LexicalScope &s) {

  for (Object *arg : arg_list) {
    Object *cond = arg->eval(r, s);
    if (!cond->null())
      return cond;
  }
  return nil::get();
}

Object *op_and(list<Object *> arg_list, LocalRuntime &r, LexicalScope &s) {

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
  if (arg_list.front()->null()) {
    return t::get();
  } else {
    return nil::get();
  }
}

Operator *op::do_or;
Operator *op::do_and;
Operator *op::do_not;

GenericFn *op::equal;
GenericFn *op::bin_equal;
GenericFn *op::greater;
GenericFn *op::bin_greater;

void op::initialize_logic() {
  Operator* in_op_equal = new InbuiltOperator(
      "=",
      "returns t if all arguments are equal, or nil otherwise",
      op_equal, type::Fn::with_rest(new type::Any), true);
  Operator* in_op_greater = new InbuiltOperator(
      ">",
      "Returns the first nil argument, or the last argument if all are non-nil",
      op_greater, type::Fn::with_rest(new type::Any), true);




  op::bin_equal = new GenericFn;
  op::bin_equal->type = type::Fn::with_args({new type::Any, new type::Any});
  op::bin_greater = new GenericFn;
  op::bin_greater->type = type::Fn::with_args({new type::Any, new type::Any});

  op::equal = new GenericFn;
  op::equal->type = type::Fn::with_rest(new type::Any);
  op::equal->add(in_op_equal);
  op::greater = new GenericFn;
  op::greater->type = type::Fn::with_rest(new type::Any);
  op::greater->add(in_op_greater);

  op::do_or = new InbuiltOperator(
      "or",
      "Returns the first non-nil argument, or nil if all arguments are nil",
      op_or, type::Fn::with_rest(new type::Any), false);
  op::do_and = new InbuiltOperator(
      "and",
      "Returns the first nil argument, or the last argument if all are non-nil",
      op_and, type::Fn::with_rest(new type::Any), false);
  op::do_not = new InbuiltOperator(
      "not", "Returns t if argument is nil, and nil otherwise", op_not,
      type::Fn::with_args({new type::Any}), true);
}
