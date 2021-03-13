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

expr::Object *Nil::check_type(expr::Object* obj) {
  return  obj->null() ? (Object*) expr::t::get() : (Object*) expr::nil::get();
}
