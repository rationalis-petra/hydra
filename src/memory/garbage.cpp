#include <iostream>
#include "expressions.hpp"

// the garbage collector for the language

using std::list;

using std::list;

void mark(runtime& r) {
  // mark all objects accessible from the root and active module
  r.root->mark_node();
  r.active_module->mark_node();

  // also, mark everything accessible via a lexical context
  for (lexical_scope* s : hydra_object::context_list) {
    for (auto o : s->map) {
      o.first->mark_node();
      o.second->mark_node();
    }
  }
  for (auto o : hydra_object::roots.data) {
    o.first->mark_node();
  }
  for (auto o : r.restarts) {
    o->mark_node();
  }

  hydra_t::get()->mark_node();
  hydra_nil::get()->mark_node();
}

void sweep() {
  list<hydra_object*> new_list;
  for (hydra_object* obj : hydra_object::node_list) {
    if (!obj->marked) {
      delete obj;
    }
    else {
      obj->marked = false;
      new_list.push_front(obj);
    }
  }
  hydra_object::node_list = new_list;
}

void hydra_object::collect_garbage(runtime& r) {
  if (counter > 10000) {
    counter = 0;

    mark(r);
    sweep();
  }
}

