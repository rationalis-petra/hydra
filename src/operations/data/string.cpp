#include <list>

#include "expressions.hpp"
#include "operations.hpp"
#include "utils.hpp"

using std::list;
using std::string;

using namespace expr;
using namespace interp;

Object *op_str_elt(list<Object *> arg_list, LocalRuntime &r, LexicalScope &s) {

  if (arg_list.size() != 2) {
    string err = "invalid number of args to elt";
    throw err;
  }
  HString *arr = dynamic_cast<HString *>(arg_list.front());
  Integer *idx = dynamic_cast<Integer *>(arg_list.back());
  if (!arr) {
    string err = "First element to str-elt should be string";
    throw err;
  }
  if (!idx) {
    string err = "Second element to str-elt should be index";
    throw err;
  }
  return new Char(arr->value.at(idx->value));
}

Object *op_str_cat(list<Object *> arg_list, LocalRuntime &r, LexicalScope &s) {

  string str;
  for (Object *obj : arg_list) {
    HString *s = dynamic_cast<HString *>(obj);
    if (s) {
      str += s->value;
    }
  }
  return new HString(str);
}

Object *op_str_gr(list<Object *> arg_list, LocalRuntime &r, LexicalScope &s) {

  HString *s1 = dynamic_cast<HString *>(arg_list.front());
  HString *s2 = dynamic_cast<HString *>(arg_list.back());
  if (s1->value > s2->value) {
    return t::get();
  } else {
    return nil::get();
  }
}

Object *op_str_eq(list<Object *> arg_list, LocalRuntime &r, LexicalScope &s) {

  HString *str1 = dynamic_cast<HString *>(arg_list.front());
  HString *str2 = dynamic_cast<HString *>(arg_list.back());

  // TODO: maybe compare slots???
  if (str1->value == str2->value) {
    return t::get();
  }
  return nil::get();
}

#include <iostream>
Object *op_char_eq(list<Object *> arg_list, LocalRuntime &r, LexicalScope &s) {

  Char *c1 = get_inbuilt<Char *>(arg_list.front());
  Char *c2 = get_inbuilt<Char *>(arg_list.back());

  // TODO: maybe compare slots???
  if (c1->value == c2->value) {
    return t::get();
  }
  return nil::get();
}

#include <iostream>
Object *op_to_str(list<Object *> arg_list, LocalRuntime &r, LexicalScope &s) {
  return new HString(arg_list.front()->to_string(r, s));
}

Operator *op::str_elt;
Operator *op::str_cat;
Operator *op::str_gr;
Operator *op::str_eq;
Operator *op::char_eq;
Operator *op::to_str;

void op::initialize_string() {

  op::str_elt = new InbuiltOperator(
                                    "string elt",
      "Takes a string and an index, and returns the character at that index",
      op_str_elt,
      type::Fn::with_all({type::string_type, type::integer_type}, nullptr,
                         type::character_type),
      true);

  op::str_cat = new InbuiltOperator(
                                    "string cat",
      "Concatenates two strings", op_str_cat,
      type::Fn::with_all({}, type::string_type, type::string_type), true);

  op::str_gr = new InbuiltOperator(
                                   "string >",
      "Returns true if the first argument is greater than the second",
      op_str_gr, type::Fn::with_args({type::string_type, type::string_type}),
      true);

  op::str_eq = new InbuiltOperator(
                                   "string =",
      "Equality test for Strings", op_str_eq,
      type::Fn::with_args({type::string_type, type::string_type}), true);

  op::char_eq = new InbuiltOperator(
                                    "char =",
      "Equality test for Characters", op_char_eq,
      type::Fn::with_args({type::character_type, type::character_type}), true);

  op::to_str = new InbuiltOperator("to-string",
      "Converts an object into a string", op_to_str,
      type::Fn::with_all({new type::Any}, nullptr, type::string_type), true);

}
