#include <iostream>
#include "expressions.hpp"

using std::string;
using std::ostream;
using std::list;
using std::atomic;

using namespace expr;

list<Object*> Object::node_list;
list<LexicalScope*> Object::context_list;
hydra_roots Object::roots; 
Runtime Object::r; 

atomic<unsigned long> Object::counter = 0;
//runtime *Object::r;

Object::Object() {
  node_list.push_front(this);
  marked = false;
  counter++;

  // collect garbage??
}

Object::~Object() {}

bool Object::null() const {
  return false;
}

// the default behaviour is to self-evaluate
Object* Object::eval(LocalRuntime& r, LexicalScope& s) {
  return this;
}

ostream &expr::operator<<(ostream &os, const Object *obj) {
  os << obj->to_string();
  return os;
}
