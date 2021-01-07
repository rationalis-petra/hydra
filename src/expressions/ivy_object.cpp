#include "expressions.hpp"

using std::string;
using std::ostream;

ivy_object::ivy_object() {
  node_list.push_front(this);
}

ivy_object::~ivy_object() {
}

bool ivy_object::null() const {
  return false;
}

// the default behaviour is to self-evaluate
ivy_object* ivy_object::eval(runtime& r) {
  return this;
}

ostream &operator<<(ostream &os, const ivy_object *obj) {
  os << obj->to_string();
  return os;
}
