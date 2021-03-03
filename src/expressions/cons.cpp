#include <string>
#include <typeinfo>
#include <iostream>

#include "expressions.hpp"

using namespace std;

void hydra_cons::mark_node() {
  if (marked) return;
  marked = true;
  car->mark_node();
  cdr->mark_node();
}

hydra_cons::hydra_cons(hydra_object *_car, hydra_object *_cdr)
    : car(_car), cdr(_cdr) {
}

hydra_object *hydra_cons::eval(runtime &r, lexical_scope &s) {
  hydra_object *oper = car->eval(r, s);
  hydra_object::roots.insert(oper);
  hydra_oper *op;
  if ((op = dynamic_cast<hydra_oper *>(oper))) {
    try {
      hydra_object *out = op->call(cdr, r, s);
      hydra_object::roots.remove(oper);
      return out;
    } catch (hydra_exception* e) {
      roots.remove(oper);
      throw e;
    }
  } else {
    string excp = "Attempted to call " + oper->to_string() +
                  " which is not an operation!";
    throw excp;
  }
}

string hydra_cons::to_string() const {
  string out = "(";
  const hydra_object *elt = this;
  while (!elt->null()) {
    if (const hydra_cons *obj = dynamic_cast<const hydra_cons *>(elt)) {
      out += obj->car->to_string();
      if (!obj->cdr->null()) {
        out += " ";
      }
      elt = obj->cdr;
    } else {
      out += "error!!";
    }
  }
  out += ")";
  return out;
}
