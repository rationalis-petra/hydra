#include <iostream>
#include <types.hpp>

#include "expressions.hpp"
#include "utils.hpp"

using namespace expr;
using namespace interp;

using std::string;

Parent* Symbol::parent;

Symbol* Symbol::symbol_no_parent(string _name) {
  Symbol* sym = new Symbol();
  sym->name = _name;
  sym->value = nullptr;
  sym->mut = true;
  return sym;
}

Symbol::Symbol() {}

Symbol::Symbol(string _name) {
  Symbol* pt = get_keyword("parent");
  parents.insert(pt);
  slots[pt] = parent;

  name = _name;
  value = nullptr;
  mut = true;
}

void Symbol::mark_node() {
  if (marked) return;
  Object::mark_node();
  if (value) {
    value->mark_node();
  }
}

string Symbol::to_string(LocalRuntime &r, LexicalScope &s) {
  return name;
}


Object* Symbol::eval(LocalRuntime& r, LexicalScope& s) {

  // fist, check the lexical scope
  auto loc = s.map.find(this);
  if (loc != s.map.end()) {
    return loc->second; 
  }

  // otherwise, return your value
  if (value == nullptr) {
    string err = "Error: symbol: " + name + " holds no value!";
    throw err;
  } else {
    return value;
  }
}

