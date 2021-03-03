#include <list>

#include "expressions.hpp"
#include "operations.hpp"

using std::list;
using std::string;

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

op_str_gr::op_str_gr() {
  is_fn = true;
  docstring = new hydra_string(
      "Returns true if the first argument is greater than the second");
  type->arg_list.push_front(new type_string);
  type->arg_list.push_front(new type_string);
}

hydra_object *op_str_gr::call(hydra_object *alist, runtime &r, lexical_scope &s) {
  list<hydra_object *> arg_list = get_arg_list(alist, r, s);
  hydra_string *s1 = dynamic_cast<hydra_string *>(arg_list.front());
  hydra_string *s2 = dynamic_cast<hydra_string *>(arg_list.back());
  if (s1->value > s2->value) {
    return hydra_t::get();
  } else {
    return hydra_nil::get(); 
  }
}
