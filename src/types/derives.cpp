#include <string>
#include <list>

#include "expressions.hpp"
#include "types.hpp"

using std::string;
using std::list;
using std::set;

using namespace type;

Derives::Derives(Object* obj) {
  object = obj;
}

void Derives::mark_node() {
  marked = true;
  object->mark_node();
}

string Derives::to_string() const {
  return "{Derives " + object->to_string() +  "}";
}


expr::Object *derive_check(set<expr::Object*> ptypes, expr::Object* obj) {
  for (auto* proto : ptypes) {
    if (proto == obj) {
      return expr::t::get();
    }
    std::set<expr::Object*> ptypes2;
    for (auto s : proto->parents) {
      ptypes2.insert(proto->slots[s]);
    }
    if (!derive_check(ptypes2, obj)->null()) {
      return expr::t::get();
    }
  }
  return expr::nil::get();
}

expr::Object *Derives::check_type(expr::Object* obj) {
  expr::Object* oobj = dynamic_cast<expr::UserObject*>(obj);
  if (oobj == nullptr) {
    return expr::nil::get();
  } else {
    // an objetc derives itself
    if (oobj == object) {
      return expr::t::get(); 
    }
    std::set<expr::Object*> ptypes;
    for (auto s : obj->parents) {
      ptypes.insert(obj->slots[s]);
    }
    return derive_check(ptypes, object);
  }
}

void DerivesConstructor::mark_node() {
  marked = true;
}


expr::Object *Derives::subtype(Type* ty) {
  if (Derives* drv = dynamic_cast<Derives*>(ty)) {
    if (drv->check_type(object)->null()) {
      return expr::nil::get();
    } else {
      return expr::t::get();
    }
  }
  return expr::nil::get();
}



Type *DerivesConstructor::constructor(list<expr::Object*> lst) {
  return new Derives(lst.front());
}

string DerivesConstructor::to_string() const {
  return "Type constructor for derives";
}

