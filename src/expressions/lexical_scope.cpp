#include <iostream>
#include "expressions.hpp"

using namespace expr;

LexicalScope::LexicalScope() {
  Value::context_list.push_front(this);
}

LexicalScope::~LexicalScope() {
  Value::context_list.remove(this);
}
