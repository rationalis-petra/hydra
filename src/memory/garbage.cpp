#include <iostream>
#include "expressions.hpp"

// the garbage collector for the language
using namespace expr;

using std::string;
using std::list;

void mark() {
  // mark all objects accessible from the root and active module
  Value::r.root->mark_node();
  if (Value::r.local_runtimes.size() > 1) {
    string err = "Error in collect_garbage: using multiple threads when this is not yet supported!!";
    throw err;
  }

  for (LocalRuntime* loc : Value::r.local_runtimes) {
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

void Value::collect_garbage() {
  if (counter > 10000) {
    counter = 0;

    mark();
    sweep();
  }
}

