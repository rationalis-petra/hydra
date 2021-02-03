#include <string>

#include "expressions.hpp"

using std::string;
using std::map;
using std::list;

hydra_module::hydra_module() {}

hydra_module::hydra_module(string _name) {
  name = _name;
}

string hydra_module::to_string() const {
  if (name == "") {
    return "<anonymous module>";
  } else {
    return "[@m " + name + "]";
  }
}

hydra_symbol *hydra_module::intern(string str) {
  hydra_object* out = get(str);
  if (out->null()) {
      hydra_symbol *sym = new hydra_symbol(str);
      symbols[str] = sym;
      return sym;
  } else {
    return static_cast<hydra_symbol*>(out);
  }
}

hydra_symbol *hydra_module::intern(list<string> path) {
  string str = path.front();
  hydra_object* out = get(str);
  path.pop_front();

  if (out->null()) {
    if (path.empty()) {
      hydra_symbol *sym = new hydra_symbol(str);
      symbols[str] = sym;
      return sym;
    } else {
      string err = "module " + str + " does not exist in module " + name;
      throw err;
    }
  } else {
    hydra_symbol* sym = hydra_cast<hydra_symbol>(out);
    if (path.empty()) {
      return sym;
    } else {
      hydra_module* mod = hydra_cast<hydra_module>(sym->value);
      return mod->intern(path);
    }
  }
}

hydra_object *hydra_module::get(string str) {
  map<string, hydra_symbol*>::iterator loc = symbols.find(str);
  if (loc == symbols.end()) {
    return hydra_nil::get();
  } else {
    return loc->second;
  }
}

hydra_object *hydra_module::get(list<string> path) {
  map<string, hydra_symbol*>::iterator loc = symbols.find(path.front());
  if (loc == symbols.end()) {
    return hydra_nil::get();
  } else {
    path.pop_front();
    if (path.empty()) {
      return loc->second;
    } else {
      if (hydra_module *mod = dynamic_cast<hydra_module*>(loc->second)) {
        return mod->get(path);
      } else {
        string err = "Symbol: " + loc->first + " does not name a module in module " + name;  
        throw err;
      }
    }
  }
}


