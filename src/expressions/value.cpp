#include <iostream>
#include "expressions.hpp"

using std::string;
using std::ostream;
using std::list;

using namespace expr;

list<Value*> Value::node_list;
list<LexicalScope*> Value::context_list;
hydra_roots Value::roots; 
Runtime Value::r; 

unsigned long Value::counter = 0;
//runtime *Value::r;

Value::Value() {
  node_list.push_front(this);
  marked = false;
  counter++;

  // check for the number of 
}

Value::~Value() {}

bool Value::null() const {
  return false;
}

// the default behaviour is to self-evaluate
Value* Value::eval(LocalRuntime& r, LexicalScope& s) {
  return this;
}

ostream &expr::operator<<(ostream &os, const Value *obj) {
  os << obj->to_string();
  return os;
}
