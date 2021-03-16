#include <string>
#include <list>

#include "expressions.hpp"
#include "types.hpp"

using std::string;
using std::list;
using std::set;

using namespace type;

Is::Is(Object* obj) {
  object = obj;
}

void Is::mark_node() {
  if (marked) return;
  Object::mark_node();
  object->mark_node();
}

string Is::to_string() const {
  return "{Is " + object->to_string() +  "}";
}


expr::Object *Is::check_type(expr::Object* obj) {
  // an objetc is itself
  if (obj == object) {
    return expr::t::get();
  }

  std::set<expr::Object *> ptypes;
  for (auto s : obj->parents) {
    ptypes.insert(obj->slots[s]);
  }
  return object->derive_check(ptypes);
}

void IsConstructor::mark_node() {
  marked = true;
}


expr::Object *Is::subtype(Type* ty) {
  // TODO: derives??
  // is derives a subtype of us, or are we a subtype of derives?
  if (Is* drv = dynamic_cast<Is*>(ty)) {
    if (drv->check_type(object)->null()) {
      return expr::nil::get();
    } else {
      return expr::t::get();
    }
  }
  return expr::nil::get();
}



Type *IsConstructor::constructor(list<expr::Object*> lst) {
  return new Is(lst.front());
}

string IsConstructor::to_string() const {
  return "Type constructor for is";
}

