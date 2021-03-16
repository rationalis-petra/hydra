#include <iostream>
#include "expressions.hpp"

using std::string;
using std::ostream;
using std::list;
using std::set;
using std::mutex;
using std::atomic;

using namespace expr;

list<Object*> Object::node_list;
list<LexicalScope*> Object::context_list;
hydra_roots Object::roots; 
Runtime Object::r; 

atomic<unsigned long> Object::counter = 0;
//runtime *Object::r;

mutex Object::root_mutex;

Object::Object() {
  std::unique_lock<mutex> lck(root_mutex);
  node_list.push_front(this);
  marked = false;
  counter++;
  invoker = nullptr;

  lck.unlock();
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

ostream &expr::operator<<(ostream &os, const Object *obj) {
  os << obj->to_string();
  return os;
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
