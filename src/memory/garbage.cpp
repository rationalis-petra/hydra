#include <iostream>
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
    } else if (hydra_oper *op = dynamic_cast<hydra_oper*>(obj)) {
      mark_obj(op->type);
      if (user_oper *uop = dynamic_cast<user_oper *>(obj)) {
        mark_obj(uop->expr);
      }
      mark_obj(op->docstring);
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
  // mark all objects accessible from the root and active module
  mark_obj(r.root);
  mark_obj(r.active_module);

  // also, mark everything accessible via a lexical context
  for (lexical_scope* s : hydra_object::context_list) {
    for (auto o : s->map) {
      mark_obj(o.first);
      mark_obj(o.second);
    }
  }
  for (hydra_object* o : hydra_object::roots) {
    mark_obj(o);
  }
}

unsigned long sweep() {
  list<hydra_object*> new_list;
  unsigned long num = 0;
  for (hydra_object* obj : hydra_object::node_list) {
    if (!obj->marked) {
      delete obj;
    }
    else {
      num++;
      obj->marked = false;
      new_list.push_front(obj);
    }
  }
  hydra_object::node_list = new_list;
  return num;
}

void hydra_object::collect_garbage(runtime& r) {
  if ((counter - last) > 10000) {
    mark(r);
    counter = node_list.size();
    last = sweep();
  }
}

