#include "expressions.hpp"

// the garbage collector for the language

using std::list;

void mark_obj(hydra_object* obj) {
  if (!obj->marked) {
    obj->marked = true;
    if (hydra_cons *cns = dynamic_cast<hydra_cons *>(obj)) {
      mark_obj(cns->car);
      mark_obj(cns->cdr);
    } else if (hydra_array *arr = dynamic_cast<hydra_array*>(obj)) {
      for (hydra_object* obj : arr->array) {
        mark_obj(obj);
      }
    } else if (user_oper *op = dynamic_cast<user_oper*>(obj)) {
      mark_obj(op->expr);
    } else if (hydra_symbol *sym = dynamic_cast<hydra_symbol*>(obj)) {
      if (sym->value) {
        mark_obj(sym->value);
      }
    } else if (hydra_module *mod = dynamic_cast<hydra_module*>(obj)) {
      for (auto valpair : mod->symbols) {
        mark_obj(valpair.second);
      }
    }
  }
}

void mark(runtime& r) {
  mark_obj(r.root);
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

