#include <list>

#include "expressions.hpp"
#include "operations.hpp"

using std::list;
using std::string;

op_cons::op_cons() {
  is_fn = true;
  docstring = new hydra_string("Creates a new cons cell and places the first "
                               "argument in the car,\n the second in the cdr."
                               "Will fail if the second is not cons or nil");
  type->arg_list.push_back(new type_nil);
  type->arg_list.push_back(new type_list);
}
hydra_object *op_cons::call(hydra_object *alist, runtime &r, lexical_scope &s) {
  list<hydra_object *> arg_list = get_arg_list(alist, r, s);
  if (arg_list.size() != 2) {
    throw "invalid number of args to cons";
  }
  hydra_object *car = arg_list.front();
  arg_list.pop_front();
  hydra_object *cdr = arg_list.front();

  if (!(dynamic_cast<hydra_cons *>(cdr) || dynamic_cast<hydra_nil *>(cdr))) {
    string err =
        "Non-list provided as second argument to cons: " + cdr->to_string();
    throw err;
  }

  hydra_cons *out = new hydra_cons(car, cdr);
  return out;
}

op_cdr::op_cdr() {
  is_fn = true;
  docstring =
      new hydra_string("Takes a cons cell as input, and returns the cdr");
  type->arg_list.push_front(new type_cons);
}
hydra_object *op_cdr::call(hydra_object *alist, runtime &r, lexical_scope &s) {
  list<hydra_object *> arg_list = get_arg_list(alist, r, s);
  if (arg_list.size() != 1) {
    throw "invalid number of args to cdr";
  }

  if (hydra_cons *cons = dynamic_cast<hydra_cons *>(arg_list.front())) {
    return cons->cdr;
  } else {
    string err =
        "Non-cons argument provided to cdr: " + arg_list.front()->to_string();
    throw err;
  }
}

op_car::op_car() {
  is_fn = true;
  docstring =
      new hydra_string("Takes a cons cell as input, and returns the car");
  type->arg_list.push_front(new type_cons);
}
hydra_object *op_car::call(hydra_object *alist, runtime &r, lexical_scope &s) {
  list<hydra_object *> arg_list = get_arg_list(alist, r, s);
  if (arg_list.size() != 1) {
    throw "invalid number of args to car";
  }

  if (hydra_cons *cons = dynamic_cast<hydra_cons *>(arg_list.front())) {
    return cons->car;
  } else {
    string err =
        "Non-cons argument provided to car: " + arg_list.front()->to_string();
    throw err;
  }
}

op_arr::op_arr() {
  is_fn = true;
  docstring =
      new hydra_string("Will return an array whose elements are the arg-list");
  type->rest_type = new type_nil;
}
hydra_object *op_arr::call(hydra_object *alist, runtime &r, lexical_scope &s) {
  list<hydra_object *> arg_list = get_arg_list(alist, r, s);

  hydra_array *out = new hydra_array;
  for (hydra_object *o : arg_list)
    out->array.push_back(o);
  return out;
}

op_elt::op_elt() {
  is_fn = true;
  docstring = new hydra_string(
      "Takes an array and an index, and returns the element at that index");
  type->arg_list.push_front(new type_integer);
  type->arg_list.push_front(new type_array);
}
hydra_object *op_elt::call(hydra_object *alist, runtime &r, lexical_scope &s) {
  list<hydra_object *> arg_list = get_arg_list(alist, r, s);
  if (arg_list.size() != 2) {
    string err = "invalid number of args to elt";
    throw err;
  }
  hydra_array *arr = dynamic_cast<hydra_array *>(arg_list.front());
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

op_str_elt::op_str_elt() {
  is_fn = true;
  docstring = new hydra_string(
      "Takes a string and an index, and returns the character at that index");
  type->arg_list.push_front(new type_integer);
  type->arg_list.push_front(new type_string);
}

hydra_object *op_str_elt::call(hydra_object *alist, runtime &r, lexical_scope &s) {
  list<hydra_object *> arg_list = get_arg_list(alist, r, s);
  if (arg_list.size() != 2) {
    string err = "invalid number of args to elt";
    throw err;
  }
  hydra_string *arr = dynamic_cast<hydra_string *>(arg_list.front());
  hydra_num *idx = dynamic_cast<hydra_num *>(arg_list.back());
  if (!arr) {
    string err = "First element to str-elt should be string";
    throw err;
  }
  if (!idx) {
    string err = "Second element to str-elt should be index";
    throw err;
  }
  return new hydra_char(arr->value.at(idx->value));
}

op_str_cat::op_str_cat() {
  is_fn = true;
  docstring = new hydra_string(
      "Concatenates two strings");
  type->rest_type = new type_string;
}

hydra_object *op_str_cat::call(hydra_object *alist, runtime &r, lexical_scope &s) {
  list<hydra_object *> arg_list = get_arg_list(alist, r, s);
  string str;
  for (hydra_object* obj : arg_list) {
    hydra_string* s = dynamic_cast<hydra_string*>(obj);
    if (s) {
      str += s->value;
    }
  }
  return new hydra_string(str);
}

op_vec_cat::op_vec_cat() {
  is_fn = true;
  docstring = new hydra_string(
      "Concatenates two vectors");
  type->rest_type = new type_array;
}

hydra_object *op_vec_cat::call(hydra_object *alist, runtime &r, lexical_scope &s) {
  list<hydra_object *> arg_list = get_arg_list(alist, r, s);
  hydra_array* vec = new hydra_array;
  for (hydra_object* obj : arg_list) {
    hydra_array* a = dynamic_cast<hydra_array*>(obj);
    if (a) {
      for (hydra_object* o : a->array) {
        vec->array.push_back(o);
      }
    }
  }
  return vec;
}

op_lock::op_lock() {
  is_fn = true;
  docstring = new hydra_string(
      "Takes a symbol, and prevents set from being used to modify its' value");
}

hydra_object *op_lock::call(hydra_object *alist, runtime &r, lexical_scope &s) {
  list<hydra_object *> arg_list = get_arg_list(alist, r, s);
  if (arg_list.size() != 1) {
    string err = "Incorrect number of arguments to lock: " +
                 std::to_string(arg_list.size()) + " expected 1";
    throw err;
  }
  hydra_cast<hydra_symbol>(arg_list.front())->mut = false;
  return arg_list.front();
}

op_unlock::op_unlock() {
  docstring = new hydra_string(
      "Takes a symbol, and will allow set to be used to modify it's value");
}

hydra_object *op_unlock::call(hydra_object *alist, runtime &r,
                              lexical_scope &s) {
  list<hydra_object *> arg_list = get_arg_list(alist, r, s);
  if (arg_list.size() != 1) {
    string err = "Incorrect number of arguments to unlock: " +
                 std::to_string(arg_list.size()) + " expected 1";
    throw err;
  }

  hydra_cast<hydra_symbol>(arg_list.front())->mut = true;
  return arg_list.front();
}
