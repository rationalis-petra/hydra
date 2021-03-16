#include <string>
#include <typeinfo>

#include "expressions.hpp"

using namespace std;
using namespace expr;


void t::mark_node() {
  Object::mark_node();
}

t* t::singleton = nullptr;
t::t() {}
t* t::get() {
  if (singleton == nullptr) {
    singleton = new t;
    roots.insert(singleton);
  }
  return singleton;
}

string t::to_string() const {
  return "t";
}
void nil::mark_node() {
  marked = true;
}

nil::nil() {};
nil* nil::singleton = nullptr;
nil* nil::get() {
  if (singleton == nullptr) {
    singleton = new nil;
    roots.insert(singleton);
  }
  return singleton;
}
bool nil::null() const {
  return true;
}

string nil::to_string() const {
  return "nil";
}
