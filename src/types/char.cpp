#include <string>

#include "expressions.hpp"
#include "types.hpp"

using std::string;
using std::list;

using type::Char;
using type::Type;

void Char::mark_node() {
  marked = true;
}

string Char::to_string() const {
  return "Char";
}

expr::Value *Char::check_type(expr::Value* obj) {
  if ((dynamic_cast<expr::Char*>(obj)) == nullptr) {
    return expr::nil::get();
  } else {
    return expr::t::get();
  }
}
