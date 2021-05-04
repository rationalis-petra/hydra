#include <string>

#include "expressions.hpp"
#include "utils.hpp"

using std::string;
using std::map;
using std::list;

using namespace expr;
using namespace interp;

Parent* Module::parent;

void ModuleImpl::mark_node() {
  if (marked) return;
  Object::mark_node();
  for (auto pair : symbols) {
    pair.second->mark_node();
  }
}

ModuleImpl::ModuleImpl() {}

ModuleImpl* ModuleImpl::no_parent(string name) {
  ModuleImpl* mod = new ModuleImpl;
  mod->name = name;
  return mod;
}

void ModuleImpl::set_parent() {
  Symbol* pt = keyword_module->intern("parent");
  parents.insert(pt);
  slots[pt] = parent;
}

ModuleImpl::ModuleImpl(string _name) {
  Symbol* pt = keyword_module->intern("parent");
  parents.insert(pt);
  slots[pt] = parent;
  name = _name;
}

string ModuleImpl::to_string(LocalRuntime &r, LexicalScope &s) {
  if (name == "") {
    return "<anonymous module>";
  } else {
    return "(module " + name + ")";
  }
}

Symbol *ModuleImpl::intern(string str) {
  Object* out = get(str);
  if (out->null()) {
      Symbol *sym = new Symbol(str);
      symbols[str] = sym;
      return sym;
  } else {
    return static_cast<Symbol*>(out);
  }
}

Symbol *ModuleImpl::intern(list<string> path) {
  string str = path.front();
  Object* out = get(str);
  path.pop_front();

  if (out->null()) {
    if (path.empty()) {
      Symbol *sym = new Symbol(str);
      symbols[str] = sym;
      return sym;
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

Object *ModuleImpl::get(string str) {
  auto it = symbols.find(str);
  if (it == symbols.end()) {
    return nil::get();
  } else {
    return it->second;
  }
}

Object *ModuleImpl::get(list<string> path) {
  map<string, Symbol*>::iterator loc = symbols.find(path.front());
  if (loc == symbols.end()) {
    return nil::get();
  } else {
    path.pop_front();
    if (path.empty()) {
      return loc->second;
    } else {
      if (Module *mod = get_inbuilt<Module*>(loc->second)) {
        return mod->get(path);
      } else {
        string err = "Symbol: " + loc->first + " does not name a module in module " + name;  
        throw err;
      }
    }
  }
}



void ModuleImpl::insert(Symbol* sym) {
  symbols[sym->name] = sym;
}

void ModuleImpl::remove(std::string str) {
  exported_symbols.erase(str);
  symbols.erase(str);
}

void ModuleImpl::export_sym(std::string str) {
  exported_symbols[str] = symbols[str];
}

Object* ModuleImpl::get_exported_symbols() {
  Object* list = nil::get();
  for (auto p : exported_symbols) {
    list = new Cons(p.second, list);
  }
  return list;
}

std::string ModuleImpl::get_name() {
  return name;
}
