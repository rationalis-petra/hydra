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

op_intern::op_intern() { eval_args = true; }
hydra_object *op_intern::call(hydra_object* alist, runtime &r) {
  list<hydra_object*> arg_list = get_arg_list(alist, r);
  if (arg_list.size() != 2) {
    string err = "invalid number of arguments provided to intern";
    throw err;
  }
  hydra_module* mod = hydra_cast<hydra_module>(arg_list.front());
  hydra_string* str = hydra_cast<hydra_string>(arg_list.back());

  return mod->intern(str->value);
}

op_insert::op_insert() { eval_args = true; }
hydra_object *op_insert::call(hydra_object* alist, runtime &r) {
  list<hydra_object*> arg_list = get_arg_list(alist, r);
  if (arg_list.size() != 2) {
    string err = "invalid number of arguments provided to insert";
    throw err;
  }
  hydra_module* mod = hydra_cast<hydra_module>(arg_list.front());
  hydra_symbol* sym = hydra_cast<hydra_symbol>(arg_list.back());

  if (!mod->get(sym->name)->null()) {
    string err = "Cannot import symbol " + sym->name + " there is already a symbol of that name";
    throw err;
  }
  mod->symbols[sym->name] = sym;
  return sym;
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

op_get_module::op_get_module() { eval_args = true; }
hydra_object *op_get_module::call(hydra_object* alist, runtime& r) {
  list<hydra_object*> arg_list = get_arg_list(alist, r);
  if (arg_list.size() != 0) {
    string err = "invalid number of arguments provided to current-module";
    throw err;
  }
  return r.active_module;
}


op_get_symbols::op_get_symbols() { eval_args = true; }
hydra_object *op_get_symbols::call(hydra_object* alist, runtime& r) {
  list<hydra_object*> arg_list = get_arg_list(alist, r);
  if (arg_list.size() != 1) {
    string err = "invalid number of arguments provided to get-symbols";
    throw err;
  }
  hydra_module* mod = hydra_cast<hydra_module>(arg_list.front());
  hydra_object* out = new hydra_nil;
  for (auto sym : mod->exported_symbols) {
    hydra_cons* cns = new hydra_cons(sym.second, out);
    out = cns;
  }
  return out;
}

// op_get_all_symbols::op_get_call_symbols() { eval_args = true; }
// hydra_object *op_get_all_symbols::call(hydra_object* alist, runtime& r) {
//   list<hydra_object*> arg_list = get_arg_list(alist, r);
//   if (arg_list.size() != 1) {
//     string err = "invalid number of arguments provided to get-symbols";
//     throw err;
//   }
//   hydra_module* mod = hydra_cast<hydra_module>(arg_list.front());
//   hydra_object* out = new hydra_nil;
//   for (auto sym : mod->symbols) {
//     hydra_cons* cns = new hydra_cons(sym.second, out);
//     out = cns;
//   }
//   return out;
// }

op_export::op_export() { eval_args = true; }
hydra_object *op_export::call(hydra_object* alist, runtime& r) {
  list<hydra_object*> arg_list = get_arg_list(alist, r);
  if (arg_list.size() != 2) {
    string err = "invalid number of arguments provided to current-module";
    throw err;
  }
  hydra_module* mod = hydra_cast<hydra_module>(arg_list.front());
  hydra_symbol* sym = hydra_cast<hydra_symbol>(arg_list.back());

  if (mod->symbols.find(sym->name) == mod->symbols.end()) {
    string err = "trying to export symbol " + sym->name +
                 " which is not in package " + mod->name;
    throw err;
  }
  else {
    mod->exported_symbols[sym->name] = sym;
  }

  return mod;
}
