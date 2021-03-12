
#include <string>

#include "expressions.hpp"
#include "types.hpp"

using std::string;

using expr::Value;
using type::Symbol;

void Symbol::mark_node() {
  marked = true;
}

string Symbol::to_string() const {
  return "Symbol";
}

Value *Symbol::check_type(Value* obj) {
  if (dynamic_cast<expr::Symbol*>(obj)) {
    return expr::t::get();
  } else {
    return expr::nil::get();
  }
}

