#include "expressions.hpp"

// the garbage collector for the language

using std::list;

list<hydra_object*> hydra_object::node_list;

hydra_object::hydra_object() {
  node_list.push_front(this);
}

void mark_obj(hydra_object* obj) {
  if (!obj->marked) {
    obj->marked = true;
    if (hydra_cons *cns = dynamic_cast<hydra_cons *>(obj)) {
      mark_obj(cns->car);
      mark_obj(cns->cdr);
    }
    else if (user_oper *op = dynamic_cast<user_oper*>(obj)) {
      mark_obj(op->expr);
    }
  }
}

void mark(runtime& r) {
  for (auto it = r.global_store.begin(); it != r.global_store.end(); it++) {
    hydra_object* obj = it->second;
    mark_obj(obj);
  }
}

void sweep() {
  list<hydra_object*> new_list;
  for (hydra_object* obj: hydra_object::node_list) {
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
  mark(r);
  sweep();
}

