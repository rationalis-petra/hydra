#include <string>
#include <list>

#include "expressions.hpp"
#include "types.hpp"

using std::string;

using namespace type;

void UserObject::mark_node() {
  marked = true;
}

string UserObject::to_string() const {
  return "Object";
}

expr::Object *UserObject::check_type(Object* obj) {
  if ((dynamic_cast<expr::UserObject *>(obj)) == nullptr) {
    return expr::nil::get();
  } else {
    return expr::t::get();
  }
}

