
#include <string>

#include "expressions.hpp"
#include "types.hpp"

using std::string;

using namespace type;

void Any::mark_node() {
  marked = true;
}

string Any::to_string() const {
  return "Any";
}

expr::Object *Any::check_type(expr::Object* obj) {
  return expr::t::get();
}

expr::Object *Any::subtype(Type *obj) {
  return expr::t::get();
}
