#include <string>

#include "expressions.hpp"
#include "types.hpp"

using std::string;
using std::list;

using type::Vector;
using type::Type;
using expr::Object;

void Vector::mark_node() {
  marked = true;
}

string Vector::to_string() const {
  return "type Vector";
}

#include <iostream>
Object *Vector::check_type(Object* obj) {
  if (dynamic_cast<expr::Vector*>(obj)) {
    return expr::t::get();
  } else {
    return expr::nil::get();
  }
}

Type *Vector::constructor(list<Object*> lst) {
  if (lst.size() == 0) {
    return this;
  } else {
    Vector* vc = new Vector();
    vc->type_elt = dynamic_cast<Type*>(lst.front());
    return vc;
  }
}

expr::Object *Vector::subtype(Type* other) {
  if (Vector* vc = dynamic_cast<Vector*>(other)) {
    return type_elt->subtype(vc->type_elt);
  } else {
    return expr::nil::get();
  }
}
