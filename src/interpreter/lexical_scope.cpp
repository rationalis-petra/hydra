#include <iostream>
#include "expressions.hpp"

using namespace expr;
using namespace interp;

LexicalScope::LexicalScope() {
  Object::collector->register_context(this);
}

LexicalScope::~LexicalScope() {
  Object::collector->remove_context(this);
}
