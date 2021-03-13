#include <iostream>
#include "expressions.hpp"

using namespace expr;

LexicalScope::LexicalScope() {
  Object::context_list.push_front(this);
}

LexicalScope::~LexicalScope() {
  Object::context_list.remove(this);
}
