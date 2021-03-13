#include <string>

#include "expressions.hpp"
#include "types.hpp"

using std::string;

using namespace type;

void Nil::mark_node() {
  marked = true;
}

string Nil::to_string() const {
  return "Nil";
}

expr::Value *Nil::check_type(expr::Value* obj) {
  return  obj->null() ? (Value*) expr::t::get() : (Value*) expr::nil::get();
}
