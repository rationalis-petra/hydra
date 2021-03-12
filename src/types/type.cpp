
#include <string>

#include "expressions.hpp"
#include "types.hpp"

using std::string;

using namespace type;

void MetaType::mark_node() {
  marked = true;
}

string MetaType::to_string() const {
  return "Type";
}

expr::Value *MetaType::check_type(expr::Value* obj) {
  if (dynamic_cast<Type*>(obj)) {
    return expr::t::get();
  } else {
    return expr::nil::get();
  }
}


void MetaConstructor::mark_node() {
  marked = true;
}

string MetaConstructor::to_string() const {
  return "{Type Constructor}";
}

expr::Value *MetaConstructor::check_type(expr::Value* obj) {
  if (dynamic_cast<TypeConstructor*>(obj)) {
    return expr::t::get();
  } else {
    return expr::nil::get();
  }
}
