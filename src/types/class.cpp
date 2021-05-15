#include <string>

#include "expressions.hpp"
#include "types.hpp"

using namespace type;
using namespace interp;

using std::string;

using expr::t;
using expr::nil;
using expr::Object;

TypeClass::TypeClass() {}

string TypeClass::to_string(LocalRuntime& r, LexicalScope &s) {
  return "typeclass";
}

expr::Object* TypeClass::check_type(expr::Object *obj) {
  return expr::t::get();
}

expr::Object* TypeClass::subtype(Type *obj) {
  return expr::t::get();
}

expr::Object* TypeClass::equal(Type *obj) {
  return expr::t::get();
}

void TypeClass::mark_node() {
  Object::mark_node();
  for (Fn* fn : ops) {
    fn->mark_node();
  }
}
