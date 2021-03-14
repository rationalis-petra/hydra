#include <string>

#include "expressions.hpp"
#include "types.hpp"

using std::string;
using std::list;

using namespace type;

void Integer::mark_node() {
  marked = true;
}

string Integer::to_string() const {
  return "Integer";
}

expr::Object *Integer::check_type(expr::Object* obj) {
  if (dynamic_cast<expr::Integer*>(obj)) {
    return expr::t::get();
  } else {
    return expr::nil::get();
  }
}

expr::Object *Integer::subtype(Type* other) {
  if (dynamic_cast<Integer*>(other)) {
    return expr::t::get(); 
  }
  return expr::nil::get();
}
