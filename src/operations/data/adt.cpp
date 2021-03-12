#include <list>

#include "expressions.hpp"
#include "operations.hpp"

using std::list;
using std::string;

using namespace expr;


Value *op_tuple(Operator* op, Value *alist, LocalRuntime &r, LexicalScope &s) {
  list<Value *> arg_list = op->get_arg_list(alist, r, s);

  Tuple *out = new Tuple;
  for (Value *o : arg_list)
    out->values.push_back(o);
  return out;
}


Value *op_tuple_elt(Operator* op, Value *alist, LocalRuntime &r, LexicalScope &s) {
  list<Value *> arg_list = op->get_arg_list(alist, r, s);

  Tuple *tup = dynamic_cast<Tuple*>(arg_list.front());
  Integer *idx = dynamic_cast<Integer*>(arg_list.back());

  return tup->values[idx->value];
}

Value *op_union(Operator* op, Value *alist, LocalRuntime &r, LexicalScope &s) {
  list<Value *> arg_list = op->get_arg_list(alist, r, s);

  Union *out = new Union;
  out->tag = dynamic_cast<Symbol*>(arg_list.front());
  out->value = arg_list.back();
  return out;
}

Operator *op::mk_tuple =
    new InbuiltOperator("Will return a tuple whose elements are the arg-list",
                        op_tuple, type::Fn::with_rest(new type::Nil), true);

Operator *op::tuple_elt =
  new InbuiltOperator("Will return the nth element of a tuple",
  op_tuple_elt, type::Fn::with_args({new type::Integer, new type::Tuple}), true);

Operator *op::mk_union = new InbuiltOperator(
    "Will return a tagged value", op_union,
    type::Fn::with_args({new type::Nil, new type::Symbol}), true);
