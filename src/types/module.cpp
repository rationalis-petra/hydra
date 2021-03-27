#include <iostream>

#include <string>

#include "expressions.hpp"
#include "types.hpp"
#include "utils.hpp"

using std::string;

using namespace interp;
using namespace type;

string Module::to_string(LocalRuntime &r, LexicalScope &s) {
  return "Module";
}

void Module::mark_node() {
  Object::mark_node();
}

expr::Object *Module::check_type(expr::Object* obj) {
  if (dynamic_cast<expr::Module*>(obj)) {
    return expr::t::get();
  } else {
    return expr::nil::get();
  }
}

expr::Object *Module::subtype(Type* ty) {
  if (dynamic_cast<Module*>(ty)) {
    return expr::t::get();
  } else {
    return expr::nil::get();
  }
}
