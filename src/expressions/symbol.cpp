#include "expressions.hpp"
#include <iostream>

using namespace expr;
using std::string;

#include <types.hpp>

void Symbol::mark_node() {
  if (marked) return;
  marked = true;
  if (value) {
    value->mark_node();
  }
}

string Symbol::to_string() const {
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

