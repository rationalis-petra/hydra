#include <string>

#include "expressions.hpp"
#include "types.hpp"

using std::string;
using std::list;

using type::Vector;
using type::Type;
using expr::Value;

void Vector::mark_node() {
  marked = true;
}

string Vector::to_string() const {
  return "Vector";
}

Value *Vector::check_type(Value* obj) {
  if ((dynamic_cast<Vector*>(obj)) || obj->null()) {
    return expr::t::get();
  } else {
    return expr::nil::get();
  }
}

Type *Vector::constructor(list<Value*> lst) {
  if (lst.size() == 0) {
    return this;
  } else {
    string err = "unimplemented...";
    throw err;
  }
}
