
#include <string>

#include "expressions.hpp"
#include "types.hpp"

using std::string;

using namespace type;

expr::Object* Type::equal(Type* other) {
  if (this == other) {
    return expr::t::get();
  } else {
    return expr::nil::get();
  }
}

void MetaType::mark_node() {
  Object::mark_node();
}

string MetaType::to_string(expr::LocalRuntime &r, expr::LexicalScope &s) {
  return "Type";
}

expr::Object *MetaType::check_type(expr::Object* obj) {
  if (dynamic_cast<Type*>(obj)) {
    return expr::t::get();
  } else {
    return expr::nil::get();
  }
}

expr::Object *MetaType::subtype(Type * ty) {
  if (dynamic_cast<MetaType*>(ty)) {
    return expr::t::get();
  } else {
    return expr::nil::get();
  }
}

void MetaConstructor::mark_node() {
  Object::mark_node();
}

string MetaConstructor::to_string(expr::LocalRuntime &r, expr::LexicalScope &s) {
  return "{Type Constructor}";
}

expr::Object *MetaConstructor::check_type(expr::Object* obj) {
  Type* t;
  if ((t = dynamic_cast<Type*>(obj)) && t->invoker) {
    return expr::t::get();
  } else {
    return expr::nil::get();
  }
}

expr::Object *MetaConstructor::subtype(Type * ty) {
  if (dynamic_cast<MetaConstructor*>(ty)) {
    return expr::t::get();
  } else {
    return expr::nil::get();
  }
}
