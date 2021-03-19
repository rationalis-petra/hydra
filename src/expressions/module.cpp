#include <string>

#include "expressions.hpp"

using std::string;
using std::map;
using std::list;
using namespace expr;

void Module::mark_node() {
  if (marked) return;
  Object::mark_node();
  for (auto pair : symbols) {
    pair.second->mark_node();
  }
}

Module::Module() {}

Module::Module(string _name) {
  name = _name;
}

string Module::to_string(LocalRuntime &r, LexicalScope &s) {
  if (name == "") {
    return "<anonymous module>";
  } else {
    return "(module " + name + ")";
  }
}

Symbol *Module::intern(string str) {
  Object* out = get(str);
  if (out->null()) {
      Symbol *sym = new Symbol(str);
      symbols[str] = sym;
      return sym;
  } else {
    return static_cast<Symbol*>(out);
  }
}

Symbol *Module::intern(list<string> path) {
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
    Symbol* sym = type::hydra_cast<Symbol>(out);
    if (path.empty()) {
      return sym;
    } else {
      Module* mod = type::hydra_cast<Module>(sym->value);
      return mod->intern(path);
    }
  }
}

Object *Module::get(string str) {
  map<string, Symbol*>::iterator loc = symbols.find(str);
  if (loc == symbols.end()) {
    return nil::get();
  } else {
    return loc->second;
  }
}

Object *Module::get(list<string> path) {
  map<string, Symbol*>::iterator loc = symbols.find(path.front());
  if (loc == symbols.end()) {
    return nil::get();
  } else {
    path.pop_front();
    if (path.empty()) {
      return loc->second;
    } else {
      if (Module *mod = dynamic_cast<Module*>(loc->second)) {
        return mod->get(path);
      } else {
        string err = "Symbol: " + loc->first + " does not name a module in module " + name;  
        throw err;
      }
    }
  }
}

