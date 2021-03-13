#include <string>
#include <list>

#include "expressions.hpp"
#include "types.hpp"

using std::string;
using std::list;
using std::set;

using namespace type;

void Object::mark_node() {
  marked = true;
}

string Object::to_string() const {
  return "Object";
}

expr::Value *Object::check_type(Value* obj) {
  if ((dynamic_cast<expr::Object *>(obj)) == nullptr) {
    return expr::nil::get();
  } else {
    return expr::t::get();
  }
}

