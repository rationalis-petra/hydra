#include <iostream>
#include "expressions.hpp"

using std::list;

lexical_scope::lexical_scope() {
  hydra_object::context_list.push_front(this);
}

lexical_scope::~lexical_scope() {
  hydra_object::context_list.remove(this);
}
