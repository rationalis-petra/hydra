#include <string>

#include "expressions.hpp"
#include "types.hpp"

using std::string;
using type::Char;
using type::Type;

void Char::mark_node() {
  marked = true;
}

string Char::to_string() const {
  return "Char";
}

expr::Object *Char::check_type(expr::Object* obj) {
  if ((dynamic_cast<expr::Char*>(obj)) == nullptr) {
    return expr::nil::get();
  } else {
    return expr::t::get();
  }
}

// is this a subtype of char?
expr::Object *Char::subtype(Type *obj) {
  if (dynamic_cast<type::Char*>(obj)) {
    return expr::t::get();
  }
  return expr::nil::get();
}
