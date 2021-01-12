#include "expressions.hpp"

using std::string;
using std::ostream;

hydra_object::hydra_object() {
  node_list.push_front(this);
  marked = false;
}

hydra_object::~hydra_object() {
}

bool hydra_object::null() const {
  return false;
}

// the default behaviour is to self-evaluate
hydra_object* hydra_object::eval(runtime& r) {
  return this;
}

ostream &operator<<(ostream &os, const hydra_object *obj) {
  os << obj->to_string();
  return os;
}
