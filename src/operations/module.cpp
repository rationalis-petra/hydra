#include <string>
#include <list>

#include "operations/modules.hpp"

using std::list;
using std::string;

op_make_symbol::op_make_symbol() { eval_args = true; }
hydra_object *op_make_symbol::call(hydra_object *alist, runtime &r) {
  list<hydra_object*> arg_list = get_arg_list(alist, r);
  if (arg_list.size() != 1) {
    string err = "invalid number of arguments to make-symbol";
    throw err;
  }
  string name = hydra_cast<hydra_string>(arg_list.front())->value;
  return new hydra_symbol(name);
}

op_make_module::op_make_module() { eval_args = true; }
hydra_object *op_make_module::call(hydra_object* alist, runtime& r) {
  list<hydra_object*> arg_list = get_arg_list(alist, r);
  if (arg_list.size() > 1) {
    string err = "invalid number of arguments to make-module";
    throw err;
  }
  if (arg_list.empty()) {
    return new hydra_module;
  }
  string name = hydra_cast<hydra_string>(arg_list.front())->value;
  return new hydra_module(name);
}

op_in_module::op_in_module() { eval_args = true; }
hydra_object *op_in_module::call(hydra_object* alist, runtime& r) {
  list<hydra_object*> arg_list = get_arg_list(alist, r);
  if (arg_list.size() != 1) {
    string err = "invalid number of arguments to in-module";
    throw err;
  }
  hydra_module* mod = hydra_cast<hydra_module>(arg_list.front());
  r.active_module = mod;
  return new hydra_t;
}

op_insert::op_insert() { eval_args = true; }
hydra_object *op_insert::call(hydra_object* alist, runtime &r) {
  list<hydra_object*> arg_list = get_arg_list(alist, r);
  if (arg_list.size() != 2) {
    string err = "invalid number of arguments provided to insert";
    throw err;
  }
  hydra_module* mod = hydra_cast<hydra_module>(arg_list.front());
  hydra_string* str = hydra_cast<hydra_string>(arg_list.back());

  return mod->intern(str->value);
}

op_get::op_get() { eval_args = true; }
hydra_object *op_get::call(hydra_object* alist, runtime& r) {
  list<hydra_object*> arg_list = get_arg_list(alist, r);
  if (arg_list.size() != 2) {
    string err = "invalid number of arguments provided to get";
    throw err;
  }
  hydra_module* mod = hydra_cast<hydra_module>(arg_list.front());
  hydra_string* str = hydra_cast<hydra_string>(arg_list.back());

  return mod->get(str->value);
}
