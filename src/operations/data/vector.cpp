#include <list>

#include "expressions.hpp"
#include "operations.hpp"

using std::list;
using std::string;

op_vec::op_vec() {
  is_fn = true;
  docstring =
      new hydra_string("Will return an array whose elements are the arg-list");
  type->rest_type = new type_nil;
}
hydra_object *op_vec::call(hydra_object *alist, runtime &r, lexical_scope &s) {
  list<hydra_object *> arg_list = get_arg_list(alist, r, s);

  hydra_vector *out = new hydra_vector;
  for (hydra_object *o : arg_list)
    out->array.push_back(o);
  return out;
}
op_vec_cat::op_vec_cat() {
  is_fn = true;
  docstring = new hydra_string(
      "Concatenates two vectors");
  type->rest_type = new type_vector;
}



hydra_object *op_vec_cat::call(hydra_object *alist, runtime &r, lexical_scope &s) {
  list<hydra_object *> arg_list = get_arg_list(alist, r, s);
  hydra_vector* vec = new hydra_vector;
  for (hydra_object* obj : arg_list) {
    hydra_vector* a = dynamic_cast<hydra_vector*>(obj);
    if (a) {
      for (hydra_object* o : a->array) {
        vec->array.push_back(o);
      }
    }
  }
  return vec;
}

op_vec_elt::op_vec_elt() {
  is_fn = true;
  docstring = new hydra_string(
      "Takes an array and an index, and returns the element at that index");
  type->arg_list.push_front(new type_integer);
  type->arg_list.push_front(new type_vector);
}

hydra_object *op_vec_elt::call(hydra_object *alist, runtime &r, lexical_scope &s) {
  list<hydra_object *> arg_list = get_arg_list(alist, r, s);
  if (arg_list.size() != 2) {
    string err = "invalid number of args to elt";
    throw err;
  }
  hydra_vector *arr = dynamic_cast<hydra_vector *>(arg_list.front());
  hydra_num *idx = dynamic_cast<hydra_num *>(arg_list.back());
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

op_vec_len::op_vec_len() {
  is_fn = true;
  docstring = new hydra_string("");
  type->arg_list.push_front(new type_vector);
}

hydra_object* op_vec_len::call(hydra_object *alist, runtime &r, lexical_scope&s) {
  list<hydra_object*> arg_list = get_arg_list(alist, r, s);

  hydra_vector* arr = dynamic_cast<hydra_vector*>(arg_list.front());

  return new hydra_num(arr->array.size());
}
