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
  return "Vector";
}

Object *Vector::check_type(Object* obj) {
  if ((dynamic_cast<Vector*>(obj)) || obj->null()) {
    return expr::t::get();
  } else {
    return expr::nil::get();
  }
}

Type *Vector::constructor(list<Object*> lst) {
  if (lst.size() == 0) {
    return this;
  } else {
    string err = "unimplemented...";
    throw err;
  }
}
