#include "expressions.hpp"
#include <iostream>
#include <types.hpp>

using namespace expr;
using namespace interp;

using std::string;


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

Symbol::Symbol(string _name) {
  name = _name;
  value = nullptr;
  mut = true;
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

