
#include <string>

#include "expressions.hpp"
#include "types.hpp"

using std::string;

using expr::Object;
using type::Symbol;

void Symbol::mark_node() {
  Object::mark_node();
}

string Symbol::to_string(expr::LocalRuntime &r, expr::LexicalScope &s) {
  return "Symbol";
}

Object *Symbol::check_type(Object* obj) {
  if (dynamic_cast<expr::Symbol*>(obj)) {
    return expr::t::get();
  } else {
    return expr::nil::get();
  }
}


expr::Object *Symbol::subtype(Type * ty) {
  if (dynamic_cast<Symbol*>(ty)) {
    return expr::t::get();
  } else {
    return expr::nil::get();
  }
}
