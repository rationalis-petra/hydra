#include <string>
#include <list>

#include "expressions.hpp"
#include "types.hpp"

using std::string;
using std::set;

using namespace type;

Derives::Derives(Object* obj) {
  object = obj;
}

void Derives::mark_node() {
  if (marked) return;
  Object::mark_node();
  object->mark_node();
}

string Derives::to_string() const {
  return "{Derives " + object->to_string() +  "}";
}


expr::Object *Derives::check_type(expr::Object* obj) {
  std::set<expr::Object *> ptypes;
  for (auto s : obj->parents) {
    ptypes.insert(obj->slots[s]);
  }
  return object->derive_check(ptypes);
}


expr::Object *Derives::subtype(Type* ty) {
  if (Derives* drv = dynamic_cast<Derives*>(ty)) {
    // an object does not dervie itself 
    if (drv->check_type(object)->null()) {
      return expr::nil::get();
    } else {
      return expr::t::get();
    }
  }
  return expr::nil::get();
}
