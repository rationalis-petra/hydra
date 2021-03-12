#include <list>

#include "expressions.hpp"
#include "operations.hpp"

using std::list;
using std::string;

using namespace expr;

Value *op_vec(Operator *op, Value *alist, LocalRuntime &r, LexicalScope &s) {
  list<Value *> arg_list = op->get_arg_list(alist, r, s);

  Vector *out = new Vector;
  for (Value *o : arg_list)
    out->array.push_back(o);
  return out;
}

Operator *op::mk_vec = new expr::InbuiltOperator(
    "Will return an array whose elements are the arg-list", op_vec,
    type::Fn::with_rest(new type::Nil), true);

Value *op_vec_cat(Operator *op, Value *alist, LocalRuntime &r,
                  LexicalScope &s) {
  list<Value *> arg_list = op->get_arg_list(alist, r, s);
  Vector *vec = new Vector;
  for (Value *obj : arg_list) {
    Vector *a = dynamic_cast<Vector *>(obj);
    if (a) {
      for (Value *o : a->array) {
        vec->array.push_back(o);
      }
    }
  }
  return vec;
}

Operator *op::vec_cat = new InbuiltOperator(
    "Concatenates two vectors", op_vec_cat,
    type::Fn::with_all({}, new type::Vector, new type::Vector), true);

Value *op_vec_elt(Operator *op, Value *alist, LocalRuntime &r,
                  LexicalScope &s) {
  list<Value *> arg_list = op->get_arg_list(alist, r, s);
  if (arg_list.size() != 2) {
    string err = "invalid number of args to elt";
    throw err;
  }
  Vector *arr = dynamic_cast<Vector *>(arg_list.front());
  Integer *idx = dynamic_cast<Integer *>(arg_list.back());
  if (!arr) {
    string err = "First element to elt should be array";
    throw err;
  }
  if (!idx) {
    string err = "Second element to elt should be index";
    throw err;
  }
  return arr->array.at(idx->value);
}

Operator *op::vec_elt = new InbuiltOperator(
    "Takes an array and an index, and returns the element at that index",
    op_vec_elt,
    type::Fn::with_all({new type::Vector, new type::Integer}, nullptr,
                       new type::Nil),
    true);
Value *op_vec_len(Operator *op, Value *alist, LocalRuntime &r,
                  LexicalScope &s) {
  list<Value *> arg_list = op->get_arg_list(alist, r, s);

  Vector *arr = dynamic_cast<Vector *>(arg_list.front());

  return new Integer(arr->array.size());
}

Operator *op::vec_len = new InbuiltOperator(
    "Returns the length of a given vector", op_vec_len,
    type::Fn::with_all({new type::Vector}, nullptr, new type::Integer), true);
