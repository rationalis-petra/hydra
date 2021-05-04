#include <string>
#include <stdio.h>

#include "ltdl.h"
#include "expressions/foreign.hpp"
#include "expressions/data/cvals.hpp"
#include "expressions/data.hpp"

#include "utils.hpp"

using std::string;
using std::list;
using namespace expr;
using namespace interp;

// LIBRARIES
ForeignLib::ForeignLib(lt_dlhandle _lib) : lib(_lib) {
  Symbol* parent = get_keyword("parent");
  slots[parent] = Module::parent;
  parents.insert(parent);
}

void ForeignLib::mark_node() {
  Object::mark_node();
}
    
string ForeignLib::to_string(LocalRuntime &r, LexicalScope &s) {
  return "<module: " + name + ">";
}

Object *ForeignLib::get(std::string str) {
  Symbol* sym = nullptr;
  auto it = symbols.find(str);
  if (it != symbols.end()) {
    sym = it->second;
  } else {
    void *addr = lt_dlsym(lib, str.c_str());
    if (addr) {
      sym = new Symbol(str);
      sym->value = new UntypedProxy(addr);
    } else {
      return nil::get();
    }
  }
  return sym;
}

Object *ForeignLib::get(std::list<std::string> str_list) {
  Symbol* sym = nullptr;
  while (!str_list.empty()) {
    sym = intern(str_list.front());
    if (Module* mod = get_inbuilt<Module*>(sym->value)) {
    } else {
      string err = "Attempted to get from";
      throw err;
    }
  }
  return sym;
}

Symbol *ForeignLib::intern(std::string str) {
  Symbol* sym = nullptr;
  auto it = symbols.find(str);
  if (it != symbols.end()) {
    sym= it->second;
  } else {
    void *addr = lt_dlsym(lib, str.c_str());
    sym = new Symbol(str);
    if (addr) {
      sym->value = new UntypedProxy(addr);
    }
  }

  return sym;
}

Symbol *ForeignLib::intern(std::list<std::string> path) {
  string str = path.front();
  Object* out = get(str);
  path.pop_front();

  if (out->null()) {
    if (path.empty()) {
      return intern(str);
    } else {
      string err = "module " + str + " does not exist in module " + name;
      throw err;
    }
  } else {
    Symbol* sym = get_inbuilt<Symbol*>(out);
    if (path.empty()) {
      return sym;
    } else {
      Module* mod = get_inbuilt<Module*>(sym->value);
      return mod->intern(path);
    }
  }

}

void ForeignLib::insert(Symbol* sym) {
  auto it = symbols.find(sym->name);
  if (it != symbols.end()) {
    auto it2 = exports.find(it->second);
    if (it2 != exports.end()) {
      exports.erase(it2);
      exports.insert(sym);
    }
  }
  symbols[sym->name] = sym;
}

void ForeignLib::remove(std::string str) {
  auto it = symbols.find(str);
  if (it != symbols.end()) {
    auto it2 = exports.find(it->second);
    if (it2 != exports.end()) {
      exports.erase(it2);
    }
    symbols.erase(it);
  }
}

void ForeignLib::export_sym(std::string str) {
  exports.insert(symbols[str]);
}

Object *ForeignLib::get_exported_symbols() {
  Object* list = nil::get();
  for (Symbol* s : exports) {
    list = new Cons(s, list);
  }
  return list;
}

std::string ForeignLib::get_name() {
  return name;
}


































ForeignOperator::ForeignOperator(CFnType* _type) {
  type = _type;
  for (auto a : _type->argtypes) {
    Operator::type->arg_list.push_back(new type::Any);
  }
  is_fn = true;
  Symbol* dsym = get_keyword("docstring");
  metadata[dsym] = new HString("A foreign operator");
}

Object *ForeignOperator::call(list<Object *> arg_list, LocalRuntime &r,
                              LexicalScope &s, bool) {
  if (arg_list.size() != type->argtypes.size()) {
    string err =
        "Error in foreign function call: arg list is not of expected size";
    throw err;
  }

  void **arg_values = new void *[arg_list.size()];

  list<Object *>::iterator vals = arg_list.begin();
  list<CType*>::iterator types = type->argtypes.begin();
  for (unsigned i = 0; i < arg_list.size(); i++) {
    Object *val = *vals;
    CType* type = *types;

    CProxy *pxy = type->get_from_object(val);
    if (pxy) {
      arg_values[i] = pxy->get_ref();
    } else {
      string err = "could not construct CProxy for object...";
      throw err;
    }

    vals++;
    types++;
  }

  ffi_arg result;

  ffi_call(&fn_def, fn_address, &result, arg_values);

  if (dynamic_cast<CVoidType*>(type->return_type)) {
    return nil::get();
  } else {
    CProxy *out = type->return_type->get_from_bits((unsigned char *)result, 0,
                                                   sizeof(result));
    return out;
  }
}
