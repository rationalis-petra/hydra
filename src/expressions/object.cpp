#include <iostream>
#include "expressions.hpp"
#include "utils.hpp"

using std::string;
using std::ostream;
using std::set;

using namespace expr;
using namespace interp;

GarbageCollector* Object::collector;

Object::Object() {
  collector->register_node(this);
  marked = false;
  invoker = nullptr;
}

void Object::mark_node() {
  if (marked) return;
  marked = true;
  if (invoker) {
    invoker->mark_node();
  }
  for (auto kvp : slots) {
    kvp.first->mark_node();
    kvp.second->mark_node();
  }
}

Object::~Object() {}

bool Object::null() const {
  return false;
}

// the default behaviour is to self-evaluate
Object* Object::eval(LocalRuntime& r, LexicalScope& s) {
  return this;
}

Object *Object::derive_check(set<expr::Object*> ptypes) {
  for (auto* proto : ptypes) {
    if (proto == this) {
      return expr::t::get();
    }
    std::set<expr::Object*> ptypes2;
    for (auto s : proto->parents) {
      ptypes2.insert(proto->slots[s]);
    }
    if (!derive_check(ptypes2)->null()) {
      return expr::t::get();
    }
  }
  return expr::nil::get();
}

string Object::to_string(LocalRuntime &r, LexicalScope &s) {
  string out = "\n{";
  if (invoker) 
    out += "invoker " + hydra_to_string(invoker, r, s);
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
