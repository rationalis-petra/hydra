#include <string>
#include <list>

#include "expressions.hpp"
#include "types.hpp"

using std::string;
using std::list;
using std::set;

using namespace type;

Derives::Derives() {
  object = new expr::UserObject();
}

void Derives::mark_node() {
  marked = true;
  object->mark_node();
}

string Derives::to_string() const {
  return "{Derives " + object->to_string() +  "}";
}


expr::Object *derive_check(set<expr::UserObject*> ptypes, expr::UserObject* obj) {
  for (auto* proto : ptypes) {
    if (proto == obj) {
      return expr::t::get();
    }
    if (!derive_check(proto->prototypes, obj)->null()) {
      return expr::t::get();
    }
  }
  return expr::nil::get();
}

expr::Object *Derives::check_type(expr::Object* obj) {
  expr::UserObject* oobj = dynamic_cast<expr::UserObject*>(obj);
  if (oobj == nullptr) {
    return expr::nil::get();
  } else {
    // an objetc derives itself
    if (oobj == object) {
      return expr::t::get(); 
    }
    set<expr::UserObject*> ptypes = oobj->prototypes;
    return derive_check(ptypes, object);
  }
}

void DerivesConstructor::mark_node() {
  marked = true;
}

Type *DerivesConstructor::constructor(list<expr::Object*> lst) {
  Derives *drv = new Derives;

  for (expr::Object *obj : lst) {
    if (auto proto = dynamic_cast<expr::UserObject *>(obj)) {
      drv->object = proto;
    } else {
      string err = "non-obejct provided to type Derives constructor";
      throw err;
    }
  }
  return drv;
}

string DerivesConstructor::to_string() const {
  return "Type constructor for derives";
}
