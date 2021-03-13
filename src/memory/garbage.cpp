#include <iostream>
#include "expressions.hpp"

// the garbage collector for the language
using namespace expr;

using std::string;
using std::list;

void mark() {
  // mark all objects accessible from the root and active module
  Object::r.root->mark_node();
  if (Object::r.local_runtimes.size() > 1) {
    string err = "Error in collect_garbage: using multiple threads when this is not yet supported!!";
    throw err;
  }

  for (LocalRuntime* loc : Object::r.local_runtimes) {
    loc->active_module->mark_node();
    for (auto o : loc->restarts) {
      o->mark_node();
    }
  }

  // also, mark everything accessible via a lexical context
  for (LexicalScope* s : Object::context_list) {
    for (auto o : s->map) {
      o.first->mark_node();
      o.second->mark_node();
    }
  }
  for (auto o : Object::roots.data) {
    o.first->mark_node();
  }

  t::get()->mark_node();
  nil::get()->mark_node();
}

void sweep() {
  list<Object*> new_list;
  for (Object* obj : Object::node_list) {
    if (!obj->marked) {
      delete obj;
    }
    else {
      obj->marked = false;
      new_list.push_front(obj);
    }
  }
  Object::node_list = new_list;
}

void Object::collect_garbage() {
  if (counter > 10000) {
    counter = 0;

    mark();
    sweep();
  }
}

