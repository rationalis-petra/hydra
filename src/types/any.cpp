#include <string>

#include "expressions.hpp"
#include "types.hpp"

using std::string;

using namespace type;
using namespace interp;

void Any::mark_node() {
  Object::mark_node();
}

string Any::to_string(LocalRuntime &r, LexicalScope &s) {
  return "Any";
}

expr::Object *Any::check_type(expr::Object* obj) {
  return expr::t::get();
}

expr::Object *Any::subtype(Type *obj) {
  return expr::t::get();
}
