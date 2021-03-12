#include <iostream>
#include "expressions.hpp"

// the garbage collector for the language
using namespace expr;

using std::list;

using std::list;

void mark(LocalRuntime& r) {
  // mark all objects accessible from the root and active module
  r.r.root->mark_node();
  for (LocalRuntime* loc : r.r.local_runtimes) {
    loc->active_module->mark_node();
    for (auto o : loc->restarts) {
      o->mark_node();
    }
  }

  // also, mark everything accessible via a lexical context
  for (LexicalScope* s : Value::context_list) {
    for (auto o : s->map) {
      o.first->mark_node();
      o.second->mark_node();
    }
  }
  for (auto o : Value::roots.data) {
    o.first->mark_node();
  }

  t::get()->mark_node();
  nil::get()->mark_node();
}

void sweep() {
  list<Value*> new_list;
  for (Value* obj : Value::node_list) {
    if (!obj->marked) {
      delete obj;
    }
    else {
      obj->marked = false;
      new_list.push_front(obj);
    }
  }
  Value::node_list = new_list;
}

void Value::collect_garbage(LocalRuntime& r) {
  if (counter > 10000) {
    counter = 0;

    mark(r);
    sweep();
  }
}

