#include <iostream>

#include <string>

#include "expressions.hpp"
#include "types.hpp"

using std::string;

using namespace type;

string Module::to_string() const {
  return "Module";
}

void Module::mark_node() {
  marked = true;
}

expr::Value *Module::check_type(expr::Value* obj) {
  if (dynamic_cast<expr::Module*>(obj)) {
    return expr::t::get();
  } else {
    return expr::nil::get();
  }
}
