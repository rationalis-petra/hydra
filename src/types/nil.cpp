#include <string>

#include "expressions.hpp"
#include "types.hpp"

using std::string;

using namespace type;

void Nil::mark_node() {
  Object::mark_node();
}

string Nil::to_string(expr::LocalRuntime &r, expr::LexicalScope &s) {
  return "Nil";
}

expr::Object *Nil::check_type(expr::Object* obj) {
  return  obj->null() ? (Object*) expr::t::get() : (Object*) expr::nil::get();
}

expr::Object *Nil::subtype(Type* ty) {
  if (dynamic_cast<Nil*>(ty)) {
    return expr::t::get();
  } else {
    return expr::nil::get();
  }
}
