#include <iostream>
#include "expressions.hpp"

using std::string;
using std::ostream;
using std::list;

list<hydra_object*> hydra_object::node_list;
list<lexical_scope*> hydra_object::context_list;
std::set<hydra_object*> hydra_object::roots; 

unsigned long hydra_object::counter = 0;
unsigned long hydra_object::last = 0;
runtime *hydra_object::r;

hydra_object::hydra_object() {
  node_list.push_front(this);
  marked = false;
  mut = false;
  counter++;

  // check for the number of 
}

hydra_object::~hydra_object() {
  counter--;
}

bool hydra_object::null() const {
  return false;
}

// the default behaviour is to self-evaluate
hydra_object* hydra_object::eval(runtime& r, lexical_scope& s) {
  return this;
}

ostream &operator<<(ostream &os, const hydra_object *obj) {
  os << obj->to_string();
  return os;
}
