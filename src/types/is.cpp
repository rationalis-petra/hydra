#include <string>
#include <list>

#include "expressions.hpp"
#include "operations.hpp"
#include "utils.hpp"
#include "types.hpp"

using std::string;
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

string Is::to_string(expr::LocalRuntime &r, expr::LexicalScope &s) {
  return "{Is " + hydra_to_string(object, r, s) +  "}";
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

#include <iostream>

expr::Object *Is::subtype(Type* ty) {
  // TODO: derives??
  // is derives a subtype of us, or are we a subtype of derives?
  if (Is* is = dynamic_cast<Is*>(ty)) {
    // their type must be a subtype of ours...
    if (!check_type(is->object)->null()) {
      return expr::t::get();
    }
  } else if (Derives* drv = dynamic_cast<Derives*>(ty)) {
    if (!check_type(drv->object)->null()) {
      return expr::t::get();
    }
  }
  return expr::nil::get();
}
