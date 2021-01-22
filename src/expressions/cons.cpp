#include <string>
#include <typeinfo>

#include "expressions.hpp"

using namespace std;

hydra_cons::hydra_cons() {}
hydra_cons::hydra_cons(hydra_object* car, hydra_object* cdr) {
  this->car = car;
  this->cdr = cdr;
}

hydra_object *hydra_cons::eval(runtime &r, lexical_scope& s) {
  hydra_object *oper = car->eval(r, s);
  hydra_oper *op;
  if ((op = dynamic_cast<hydra_oper*>(oper))) {
    return op->call(cdr, r, s);
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
