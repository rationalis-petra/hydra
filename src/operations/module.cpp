#include <list>
#include <string>

#include "operations/modules.hpp"

using std::list;
using std::string;

using namespace expr;
using type::hydra_cast;

Object *op_mk_symbol(list<Object*> arg_list, LocalRuntime &r,
                    LexicalScope &s) {
  
  if (arg_list.size() != 1) {
    string err = "invalid number of arguments to make-symbol";
    throw err;
  }
  string name = hydra_cast<HString>(arg_list.front())->value;
  return new Symbol(name);
}

Operator *op::mk_symbol = new InbuiltOperator(
    "Generates a new, unique symbol whose name is the provided string",
    op_mk_symbol,
    type::Fn::with_all({new type::TString}, nullptr, new type::Symbol), true);

Object *op_mk_module(list<Object*> arg_list, LocalRuntime &r,
                    LexicalScope &s) {
  
  if (arg_list.size() > 1) {
    string err = "invalid number of arguments to make-module";
    throw err;
  }
  if (arg_list.empty()) {
    return new Module;
  }
  string name = hydra_cast<HString>(arg_list.front())->value;
  return new Module(name);
}

Operator *op::mk_module = new InbuiltOperator(
    "Generates a new module whose name is the provided string", op_mk_module,
    type::Fn::with_all({new type::TString}, nullptr, new type::Module), true);

Object *op_in_module(list<Object*> arg_list, LocalRuntime &r,
                    LexicalScope &s) {
  
  if (arg_list.size() != 1) {
    string err = "invalid number of arguments to in-module";
    throw err;
  }
  Module *mod = hydra_cast<Module>(arg_list.front());
  r.active_module = mod;
  return t::get();
}

Operator *op::in_module = new InbuiltOperator(
    "Sets the current (active) module to the provided argument", op_in_module,
    type::Fn::with_all({new type::Module}, nullptr, new type::Module), true);

Object *op_intern(list<Object*> arg_list, LocalRuntime &r, LexicalScope &s) {
  
  if (arg_list.size() != 2) {
    string err = "invalid number of arguments provided to intern";
    throw err;
  }
  Module *mod = hydra_cast<Module>(arg_list.front());
  HString *str = hydra_cast<HString>(arg_list.back());

  return mod->intern(str->value);
}

Operator *op::intern = new InbuiltOperator(
    "Take a module and a string; if a symbol with that name exists\n"
    "then return it, otherwise place a new symbol in the current\n"
    "package with that name and return it",
    op_intern,
    type::Fn::with_all({new type::Module, new type::Symbol}, nullptr,
                       new type::Symbol),
    true);

Object *op_insert(list<Object*> arg_list, LocalRuntime &r, LexicalScope &s) {
  
  if (arg_list.size() != 2) {
    string err = "invalid number of arguments provided to insert";
    throw err;
  }
  Module *mod = hydra_cast<Module>(arg_list.front());
  Symbol *sym = hydra_cast<Symbol>(arg_list.back());

  if (Object *obj = mod->get(sym->name)) {
    if (!obj->null()) {
      Symbol *sym = dynamic_cast<Symbol *>(obj);
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

Operator *op::insert = new InbuiltOperator(
    "Takes a module and a symbol; insert the symbol into the module,\n"
    "or return an error if a symbol with that name already exists in\n"
    "current module",
    op_insert,
    type::Fn::with_all({new type::Module, new type::Symbol}, nullptr,
                       new type::Symbol),
    true);

Object *op_get(list<Object*> arg_list, LocalRuntime &r, LexicalScope &s) {
  
  if (arg_list.size() != 2) {
    string err = "invalid number of arguments provided to get";
    throw err;
  }
  Module *mod = hydra_cast<Module>(arg_list.front());
  HString *str = hydra_cast<HString>(arg_list.back());

  return mod->get(str->value);
}

Operator *op::get = new InbuiltOperator(
    "Takes a module and a string; if a symbol with the name exists in"
    "the module, return it. Otherwise, return nil",
    op_get, type::Fn::with_all({new type::TString}, nullptr, new type::Symbol),
    true);

Object *op_get_module(list<Object*> arg_list, LocalRuntime &r,
                     LexicalScope &s) {
  
  if (arg_list.size() != 0) {
    string err = "invalid number of arguments provided to current-module";
    throw err;
  }
  return r.active_module;
}

Operator *op::get_module = new InbuiltOperator(
    "Returns the current (active) module", op_get_module, new type::Fn, true);

Object *op_get_symbols(list<Object*> arg_list, LocalRuntime &r,
                      LexicalScope &s) {
  
  if (arg_list.size() != 1) {
    string err = "invalid number of arguments provided to get-symbols";
    throw err;
  }
  Module *mod = hydra_cast<Module>(arg_list.front());
  Object *out = nil::get();
  for (auto sym : mod->exported_symbols) {
    Cons *cns = new Cons(sym.second, out);
    out = cns;
  }
  return out;
}

Operator *op::get_symbols = new InbuiltOperator(
    "Returns a list of symbols which are exported by a module", op_get_symbols,
    type::Fn::with_all({new type::Module}, nullptr, new type::List), true);

Object *op_remove(list<Object*> arg_list, LocalRuntime &r, LexicalScope &s) {
  
  if (arg_list.size() != 2) {
    string err = "invalid number of arguments provided to remove";
    throw err;
  }
  Module *mod = hydra_cast<Module>(arg_list.front());
  Symbol *sym = hydra_cast<Symbol>(arg_list.back());
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

Operator *op::remove = new InbuiltOperator(
    "Removes a symbol from a module, returns the module", op_remove,
    type::Fn::with_args({new type::Module, new type::TString}), true);

// op_get_all_symbols::op_get_call_symbols() { eval_args = true; }
// Object *op_get_all_symbols::call(Object* alist, LocalRuntime& r) {
//   list<Object*> arg_list = get_arg_list(alist, r);
//   if (arg_list.size() != 1) {
//     string err = "invalid number of arguments provided to get-symbols";
//     throw err;
//   }
//   Module* mod = hydra_cast<Module>(arg_list.front());
//   Object* out = new hydra_nil;
//   for (auto sym : mod->symbols) {
//     hydra_cons* cns = new hydra_cons(sym.second, out);
//     out = cns;
//   }
//   return out;
// }

Object *op_export(list<Object*> arg_list, LocalRuntime &r, LexicalScope &s) {
  
  if (arg_list.size() != 2) {
    string err = "invalid number of arguments provided to current-module";
    throw err;
  }
  Module *mod = hydra_cast<Module>(arg_list.front());
  Symbol *sym = hydra_cast<Symbol>(arg_list.back());

  if (mod->symbols.find(sym->name) == mod->symbols.end()) {
    string err = "trying to export symbol " + sym->name +
                 " which is not in package " + mod->name;
    throw err;
  } else {
    mod->exported_symbols[sym->name] = sym;
  }

  return mod;
}

Operator *op::do_export = new InbuiltOperator(
    "Moves a symbol into a modules' list of exported symbols", op_export,
    type::Fn::with_all({new type::Module, new type::Symbol}, nullptr,
                       new type::Module),
    true);
