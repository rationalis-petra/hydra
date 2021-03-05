#include <string>
#include <list>

#include "expressions.hpp"
#include "types.hpp"

using std::string;
using std::list;
using std::set;

void type_derives::mark_node() {
  marked = true;
  object->mark_node();
}

string type_derives::to_string() const {
  return "{Derives " + object->to_string() +  "}";
}


hydra_object *derive_check(set<hydra_object_object*> ptypes, hydra_obejct_object* obj) {
  for (auto* proto : ptypes) {
    if (proto == obj) {
      return hydra_t::get();
    }
    if (!derive_check(proto, obj)->null()) {
      return hydra_t::get();
    }
  }
  return hydra_nil::get();
}

hydra_object *type_cons::check_type(hydra_object* obj) {
  hydra_object_object* oobj = dynamic_cast<hydra_object_object*>(obj);
  if (oobj == nullptr) {
    return hydra_nil::get();
  } else {
    // an objetc derives itself
    if (oobj == object) {
      return hydra_t::get(); 
    }
    set<hydra_object_object*> ptypes = oobj->prototypes;
    return derive_check(ptypes, object);
  }
}

hydra_type *type_cons::constructor(list<hydra_object*> lst) {
  if (lst.size() == 0) {
    return this;
  } else {
    string err = "Cannot provide argument to the Char Type constructor";
    throw err;
  }
}
