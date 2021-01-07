#include "expressions.hpp"

// the garbage collector for the language

using std::list;

list<ivy_object*> ivy_object::node_list;

ivy_object::ivy_object() {
  node_list.push_front(this);
}

void mark_obj(ivy_object* obj) {
  if (!obj->marked) {
    obj->marked = true;
    if (ivy_cons *cns = dynamic_cast<ivy_cons *>(obj)) {
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
    ivy_object* obj = it->second;
    mark_obj(obj);
  }
}

void sweep() {
  list<ivy_object*> new_list;
  for (ivy_object* obj: ivy_object::node_list) {
    if (!obj->marked) {
      delete obj;
    }
    else {
      obj->marked = false;
      new_list.push_front(obj);
    }
  }
  ivy_object::node_list = new_list;
}

void ivy_object::collect_garbage(runtime& r) {
  mark(r);
  sweep();
}

