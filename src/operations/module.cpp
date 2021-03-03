#include <string>
#include <list>

#include "operations/modules.hpp"

using std::list;
using std::string;

op_make_symbol::op_make_symbol() {
  is_fn = true;
  docstring = new hydra_string("Generates a new, unique symbol whose name is the provided string");
  type->arg_list.push_front(new type_string);
}
hydra_object *op_make_symbol::call(hydra_object *alist, runtime &r, lexical_scope &s) {
  list<hydra_object*> arg_list = get_arg_list(alist, r, s);
  if (arg_list.size() != 1) {
    string err = "invalid number of arguments to make-symbol";
    throw err;
  }
  string name = hydra_cast<hydra_string>(arg_list.front())->value;
  return new hydra_symbol(name);
}

op_make_module::op_make_module() {
  is_fn = true;
  docstring = new hydra_string("Generates a new module whose name is the provided string");
  type->arg_list.push_front(new type_string);
}
hydra_object *op_make_module::call(hydra_object* alist, runtime& r, lexical_scope &s) {
  list<hydra_object*> arg_list = get_arg_list(alist, r, s);
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

op_in_module::op_in_module() {
  is_fn = true;
  docstring = new hydra_string("Sets the current (active) module to the provided argument");
  type->arg_list.push_front(new type_module);
}
hydra_object *op_in_module::call(hydra_object* alist, runtime& r, lexical_scope &s) {
  list<hydra_object*> arg_list = get_arg_list(alist, r, s);
  if (arg_list.size() != 1) {
    string err = "invalid number of arguments to in-module";
    throw err;
  }
  hydra_module* mod = hydra_cast<hydra_module>(arg_list.front());
  r.active_module = mod;
  return hydra_t::get();
}

op_intern::op_intern() {
  is_fn = true;
  docstring = new hydra_string("Take a module and a string; if a symbol with that name exists\n"
                               "then return it, otherwise place a new symbol in the current\n"
                               "package with that name and return it");
  type->arg_list.push_front(new type_symbol);
  type->arg_list.push_front(new type_module);
}
hydra_object *op_intern::call(hydra_object* alist, runtime &r, lexical_scope &s) {
  list<hydra_object*> arg_list = get_arg_list(alist, r, s);
  if (arg_list.size() != 2) {
    string err = "invalid number of arguments provided to intern";
    throw err;
  }
  hydra_module* mod = hydra_cast<hydra_module>(arg_list.front());
  hydra_string* str = hydra_cast<hydra_string>(arg_list.back());

  return mod->intern(str->value);
}

op_insert::op_insert() {
  is_fn = true;
  docstring = new hydra_string("Takes a module and a symbol; insert the symbol into the module,\n"
                               "or return an error if a symbol with that name already exists in\n"
                               "current module");
  type->arg_list.push_front(new type_symbol);
  type->arg_list.push_front(new type_module);
}
hydra_object *op_insert::call(hydra_object* alist, runtime &r, lexical_scope &s) {
  list<hydra_object*> arg_list = get_arg_list(alist, r, s);
  if (arg_list.size() != 2) {
    string err = "invalid number of arguments provided to insert";
    throw err;
  }
  hydra_module* mod = hydra_cast<hydra_module>(arg_list.front());
  hydra_symbol* sym = hydra_cast<hydra_symbol>(arg_list.back());

  if (hydra_object* obj = mod->get(sym->name)) {
    if (!obj->null()) {
      hydra_symbol* sym = dynamic_cast<hydra_symbol*>(obj);
      if (sym->value) {
        string err = "Cannot import symbol " + sym->name +
                     " there is already a symbol of that name";
        throw err;
      }
    }
  }
  mod->symbols[sym->name] = sym;
  return sym;
}

op_get::op_get() {
  is_fn = true;
  docstring = new hydra_string("Takes a module and a string; if a symbol with the name exists in"
                               "the module, return it. Otherwise, return nil");
  type->arg_list.push_front(new type_string);
  type->arg_list.push_front(new type_module);
}
hydra_object *op_get::call(hydra_object* alist, runtime& r, lexical_scope &s) {
  list<hydra_object*> arg_list = get_arg_list(alist, r, s);
  if (arg_list.size() != 2) {
    string err = "invalid number of arguments provided to get";
    throw err;
  }
  hydra_module* mod = hydra_cast<hydra_module>(arg_list.front());
  hydra_string* str = hydra_cast<hydra_string>(arg_list.back());

  return mod->get(str->value);
}

op_get_module::op_get_module() {
  is_fn = true;
  docstring = new hydra_string("Returns the current (active) module");
}
hydra_object *op_get_module::call(hydra_object* alist, runtime& r, lexical_scope &s) {
  list<hydra_object*> arg_list = get_arg_list(alist, r, s);
  if (arg_list.size() != 0) {
    string err = "invalid number of arguments provided to current-module";
    throw err;
  }
  return r.active_module;
}


op_get_symbols::op_get_symbols() {
  is_fn = true;
  docstring = new hydra_string("Returns a list of symbols which are exported by a module");
  type->arg_list.push_front(new type_module);
}
hydra_object *op_get_symbols::call(hydra_object* alist, runtime& r, lexical_scope &s) {
  list<hydra_object*> arg_list = get_arg_list(alist, r, s);
  if (arg_list.size() != 1) {
    string err = "invalid number of arguments provided to get-symbols";
    throw err;
  }
  hydra_module* mod = hydra_cast<hydra_module>(arg_list.front());
  hydra_object* out = hydra_nil::get();
  for (auto sym : mod->exported_symbols) {
    hydra_cons* cns = new hydra_cons(sym.second, out);
    out = cns;
  }
  return out;
}

op_remove::op_remove() {
  is_fn = true;
  docstring = new hydra_string("Removes a symbol from a module, returns the module");
  type->arg_list.push_front(new type_module);
  type->arg_list.push_front(new type_string);
}
hydra_object *op_remove::call(hydra_object* alist, runtime& r, lexical_scope &s) {
  list<hydra_object*> arg_list = get_arg_list(alist, r, s);
  if (arg_list.size() != 2) {
    string err = "invalid number of arguments provided to remove";
    throw err;
  }
  hydra_module* mod = hydra_cast<hydra_module>(arg_list.front());
  hydra_symbol* sym = hydra_cast<hydra_symbol>(arg_list.back());
  auto it = mod->symbols.find(sym->name);
  if (it != mod->symbols.end()) {
    mod->symbols.erase(it);
  }
  it = mod->exported_symbols.find(sym->name);
  if (it != mod->exported_symbols.end()) {
    mod->exported_symbols.erase(it);
  }
  return mod;
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

op_export::op_export() {
  is_fn = true;
  docstring = new hydra_string("Moves a symbol into a modules' list of exported symbols");
  type->arg_list.push_front(new type_symbol);
  type->arg_list.push_front(new type_module);
}
hydra_object *op_export::call(hydra_object* alist, runtime& r, lexical_scope &s) {
  list<hydra_object*> arg_list = get_arg_list(alist, r, s);
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
