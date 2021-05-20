#include <iostream>
#include "expressions.hpp"
#include "utils.hpp"

using std::string;
using std::ostream;
using std::set;

using namespace expr;
using namespace interp;

GarbageCollector* Object::collector;
Parent* Object::common_behaviour;
Parent* Object::default_behaviour;

Object::Object() {
  collector->register_node(this);
  marked = false;
  contract = nullptr;
}

Object::~Object() {}

void Object::mark_node() {
  if (marked) return;
  marked = true;
  for (auto kvp : slots) {
    kvp.first->mark_node();
    kvp.second->mark_node();
  }
  for (auto kvp : metadata) {
    kvp.first->mark_node();
    kvp.second->mark_node();
  }
}


bool Object::null() const {
  return false;
}

// the default behaviour is to self-evaluate
Object* Object::eval(LocalRuntime& r, LexicalScope& s) {
  return this;
}

Object *Object::derive_check(set<expr::Object*> ptypes, set<expr::Object*> visited) {
  for (auto* proto : ptypes) {
    if (proto == this) {
      return expr::t::get();
    } if (visited.find(proto) == visited.end()) {
      visited.insert(proto);

      std::set<expr::Object *> ptypes2;
      for (auto s : proto->parents) {
        ptypes2.insert(proto->slots[s]);
      }
      if (!derive_check(ptypes2, visited)->null()) {
        return t::get();
      }
    }
  }
  return expr::nil::get();
}

string Object::to_string(LocalRuntime &r, LexicalScope &s) {
  string out = "\n{";
  for (auto x : slots) {
    out += "[";
    out += hydra_to_string(x.first, r, s);
    out += " ";
    out += hydra_to_string(x.second, r, s);

    out += "]";
    if (x != *(--slots.end())) {
      out += "\n";
    }
  }
  out += "}";
  return out;
}

Object *Object::reflect(LocalRuntime& r, LexicalScope &s) {
  // TODO: check if reflect is implemented??
  return new Mirror(this);
}

Object *Object::get_slots(LocalRuntime& r, LexicalScope &s) {
  Object* cns = nil::get();
  for (auto kvp : slots) {
    cns = new Cons(kvp.first, cns);
  }
  return cns;
}

Object *Object::get_slot_value(Symbol* sym, LocalRuntime& r, LexicalScope&s) {
  auto it = slots.find(sym);
  if (it != slots.end()) {
    return it->second;
  } else {
    string err = "Cannot find slot value: " + hydra_to_string(sym, r, s);
    throw err;
  }
}

Object *Object::set_slot_value(Symbol* sym,
                               Object* val,
                               interp::LocalRuntime &r,
                               interp::LexicalScope &s) {
  slots[sym] = val;
  return val;
}

Object *Object::delete_slot(Symbol* sym, interp::LocalRuntime &r, interp::LexicalScope &s) {
  auto it = parents.find(sym);
  if (it != parents.end()) {
    parents.erase(it);
  }
  slots.erase(sym);
  return nil::get();
}

Object *Object::get_meta(interp::LocalRuntime &r, interp::LexicalScope &s) {
  Object* cns = nil::get();
  for (auto kvp : metadata) {
    cns = new Cons(kvp.first, cns);
  }
  return cns;
}

Object *Object::get_meta_value(Symbol* sym,
                               interp::LocalRuntime &r,
                               interp::LexicalScope &s) {
  auto it = metadata.find(sym);
  if (it != metadata.end()) {
    return it->second;
  } else {
    string err = "Cannot find meta-slot value: " + hydra_to_string(sym, r, s);
    throw err;
  }
}

Object *Object::set_meta_value(Symbol* sym,
                               Object* val,
                               interp::LocalRuntime &r,
                               interp::LexicalScope &s) {
  metadata[sym] = val;
  return val;
}

Object *Object::delete_meta(Symbol* sym, interp::LocalRuntime &r, interp::LexicalScope &s) {
  metadata.erase(sym);
  return nil::get();
}


void Object::set_invoker(Operator* op) {
  Symbol* insym = get_keyword("invoker");
  slots[insym] = op;
  parents.insert(insym);
}
