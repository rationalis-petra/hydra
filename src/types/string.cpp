
#include <string>

#include "expressions.hpp"
#include "types.hpp"

using std::string;

using type::TString;
 
void TString::mark_node() {
  marked = true;
}

string TString::to_string() const {
  return "String";
}

expr::Object *TString::check_type(expr::Object* obj) {
  if (dynamic_cast<expr::HString*>(obj)) {
    return expr::t::get();
  } else {
    return expr::nil::get();
  }
}

expr::Object *TString::subtype(Type * ty) {
  if (dynamic_cast<TString*>(ty)) {
    return expr::t::get();
  } else {
    return expr::nil::get();
  }
}
