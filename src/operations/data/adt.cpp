#include <list>

#include "expressions.hpp"
#include "operations.hpp"
#include "utils.hpp"

using std::list;
using std::string;

using namespace expr;
using namespace interp;

Object *op_tuple(list<Object *> arg_list, LocalRuntime &r, LexicalScope &s) {

  Object::collector->collect_garbage(r);

  Tuple *out = new Tuple();
  for (Object *o : arg_list)
    out->values.push_back(o);
  return out;
}

Object *op_tuple_eq(list<Object *> arg_list, LocalRuntime &r, LexicalScope &s) {

  Tuple *tup1 = dynamic_cast<Tuple *>(arg_list.front());
  Tuple *tup2 = dynamic_cast<Tuple *>(arg_list.back());

  if (tup1->values.size() != tup2->values.size()) {
    return nil::get();
  } else {
    for (int i = 0; i < tup1->size(); i++) {
      if (equal_operator->call({(*tup1)[i], (*tup2)[i]}, r, s)->null()) {
        return nil::get();
      }
    }
    return t::get();
  }
}

Object *op_tuple_elt(list<Object *> arg_list, LocalRuntime &r,
                     LexicalScope &s) {

  Tuple *tup = dynamic_cast<Tuple *>(arg_list.front());
  Integer *idx = dynamic_cast<Integer *>(arg_list.back());

  size_t a = idx->get_ul();
  if (a < tup->values.size()) {
    return tup->values[a];
  } else {
    string err = "Attempting to get out-of-bounds index of tuple: " +
      hydra_to_string(idx, r, s);
    throw err;
  }
}

Object *op_union(list<Object *> arg_list, LocalRuntime &r, LexicalScope &s) {

  Union *out = new Union;
  out->tag = dynamic_cast<Symbol *>(arg_list.front());
  out->value = arg_list.back();
  return out;
}

Operator *op::mk_tuple;
Operator *op::mk_union;

void op::initialize_tuple() {

  op::mk_tuple = new InbuiltOperator(
      "tuple", "Will return a tuple whose elements are the arg-list", op_tuple,
      type::Fn::with_rest(new type::Any), true);

  Operator* in_tuple_eq = new InbuiltOperator(
      "tuple =", "Equality test for Tuples", op_tuple_eq,
      type::Fn::with_args({new type::Tuple, new type::Tuple}), true);
  op::bin_equal->add(in_tuple_eq);

  Operator* in_tuple_elt = new InbuiltOperator(
      "tuple elt", "Will return the nth element of a tuple", op_tuple_elt,
      type::Fn::with_args({new type::Tuple, type::integer_type}), true);
  op::get->add(in_tuple_elt);
   

  op::mk_union = new InbuiltOperator(
      "union", "Will return a tagged value", op_union,
      type::Fn::with_args({new type::Any, type::symbol_type}), true);

}
