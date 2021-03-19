#include <string>

#include "expressions.hpp"
#include "types.hpp"

using std::string;

using type::Vector;
using type::Type;
using expr::Object;


Vector::Vector() {
  type_elt = new type::Any();
}

Vector::Vector(Type* t) {
  type_elt = t;
}

Vector::Vector(Type* t, int i) {
  type_elt = t;
  // TODO: 
}

void Vector::mark_node() {
  if (marked) return;
  Object::mark_node();
  type_elt->mark_node();
}

string Vector::to_string(expr::LocalRuntime &r, expr::LexicalScope &s) {
  return "type Vector";
}

Object *Vector::check_type(Object* obj) {
  if (dynamic_cast<expr::Vector*>(obj)) {
    return expr::t::get();
  } else {
    return expr::nil::get();
  }
}

expr::Object *Vector::subtype(Type* other) {
  if (Vector* vc = dynamic_cast<Vector*>(other)) {
    return type_elt->subtype(vc->type_elt);
  } else {
    return expr::nil::get();
  }
}
