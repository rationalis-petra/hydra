#include <list>

#include "expressions.hpp"
#include "operations.hpp"
#include "utils.hpp"

using std::list;
using std::string;

using namespace expr;
using namespace interp;

Object *op_vec(list<Object *> arg_list, LocalRuntime &r, LexicalScope &s) {

  Vector *out = new Vector;
  for (Object *o : arg_list)
    out->array.push_back(o);
  return out;
}

Object *op_vec_cat(list<Object *> arg_list, LocalRuntime &r, LexicalScope &s) {

  Vector *vec = new Vector;
  for (Object *obj : arg_list) {
    Vector *a = get_inbuilt<Vector *>(obj);
    if (a) {
      for (Object *o : a->array) {
        vec->array.push_back(o);
      }
    }
  }
  return vec;
}

Object *op_vec_elt(list<Object *> arg_list, LocalRuntime &r, LexicalScope &s) {
  Vector *arr = get_inbuilt<Vector *>(arg_list.front());
  Integer *idx = get_inbuilt<Integer *>(arg_list.back());
  size_t i = idx->value.get_ui();

  if (i < arr->array.size()) {
    return arr->array[i];
  } else {
    string err = "out of bounds index to vector: " + idx->value.get_str();
    throw err;
  }
}

Object *op_vec_len(list<Object *> arg_list, LocalRuntime &r, LexicalScope &s) {

  Vector *arr = get_inbuilt<Vector *>(arg_list.front());

  return new Integer(arr->array.size());
}

Object *op_vec_set(list<Object *> arg_list, LocalRuntime &r, LexicalScope &s) {

  Vector *arr = get_inbuilt<Vector *>(arg_list.front());
  arg_list.pop_front();
  Integer *idx = get_inbuilt<Integer *>(arg_list.front());
  arg_list.pop_front();
  Object* obj = arg_list.front();

  size_t i = idx->value.get_ui();
  if (i < arr->array.size()) {
    arr->array[i] = obj;
    return arr;
  } else {
    string err = "out of bounds index to vector: " + idx->value.get_str();
    throw err;
  }
}

Object *op_vec_eq(list<Object *> arg_list, LocalRuntime &r, LexicalScope &s) {

  Vector *vec1 = get_inbuilt<Vector *>(arg_list.front());
  Vector *vec2 = get_inbuilt<Vector *>(arg_list.back());

  if (vec1->size() == vec2->size()) {
    for (int i = 0; i < vec1->size(); i++) {
      if (equal_operator->call({(*vec1)[i], (*vec2)[i]}, r, s)->null()) {
        return nil::get();
      }
    }
    return t::get();
  }
  return nil::get();
}

Operator *op::mk_vec;

void op::initialize_vector() {
  op::mk_vec = new expr::InbuiltOperator(
      "vector", "Will return an array whose elements are the arg-list", op_vec,
      type::Fn::with_rest(new type::Any), true);


  // GENERIC FUNCTIONS
  Operator* in_vec_elt = new InbuiltOperator(
      "vector get",
      "Takes an array and an index, and returns the element at that index",
      op_vec_elt,
      type::Fn::with_all({new type::Vector, type::integer_type}, nullptr,
                         new type::Any),
      true);
  op::get->add(in_vec_elt);
  Operator* in_vec_set = new InbuiltOperator(
      "vector set",
      "Takes an array, an index, and an element. Sets the value at index to "
      "element ",
      op_vec_elt,
      type::Fn::with_all({new type::Vector, type::integer_type, new type::Any},
                         nullptr, new type::Any),
      true);
  op::set->add(in_vec_set);
  Operator* in_vec_len = new InbuiltOperator(
      "vector len", "Returns the length of a given vector", op_vec_len,
      type::Fn::with_all({new type::Vector}, nullptr, type::integer_type),
      true);
  op::len->add(in_vec_len);
  Operator* in_vec_cat = new InbuiltOperator(
      "vector concat", "Concatenates two vectors", op_vec_cat,
      type::Fn::with_all({}, new type::Vector, new type::Vector), true);
  op::cat->add(in_vec_cat);
  Operator* in_vec_eq = new InbuiltOperator(
      "vector =", "Equality test for Vectors", op_vec_eq,
      type::Fn::with_args({new type::Vector, new type::Vector}), true);
  op::bin_equal->add(in_vec_eq);
}
