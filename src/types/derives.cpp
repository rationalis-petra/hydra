#include <string>
#include <list>

#include "expressions.hpp"
#include "types.hpp"
#include "utils.hpp"

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

string Derives::to_string(expr::LocalRuntime &r, expr::LexicalScope &s) {
  return "{Derives " + hydra_to_string(object, r, s) +  "}";
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
    // subtupe if they are the same obejct...
    if (drv->object == object) {
      return expr::t::get();
      // or if their obect derives from ours
    } if (!check_type(drv->object)->null()) {
      return expr::t::get();
    } 
  } else if (Is* is = dynamic_cast<Is*>(ty)) {
    // if the is has an object which descends from 
    // us, we are ok :)
    if (!check_type(is->object)->null()) {
      return expr::t::get();
    }
  }
  return expr::nil::get();
}
