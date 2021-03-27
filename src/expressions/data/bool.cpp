#include <string>
#include <typeinfo>

#include "expressions.hpp"
#include "interpreter.hpp"

using namespace expr;
using namespace interp;

using std::string;

void t::mark_node() {
  Object::mark_node();
}

t* t::singleton = nullptr;
t::t() {}
t* t::get() {
  if (singleton == nullptr) {
    singleton = new t;
    collector->insert_root(singleton);
  }
  return singleton;
}

string t::to_string(LocalRuntime &r, LexicalScope& s) {
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
    collector->insert_root(singleton);
  }
  return singleton;
}
bool nil::null() const {
  return true;
}

string nil::to_string(LocalRuntime &r, LexicalScope& s) {
  return "nil";
}
