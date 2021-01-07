#include <string>
#include <typeinfo>

#include "expressions.hpp"

using namespace std;

ivy_object *ivy_cons::eval(runtime &r) {
  ivy_object *oper = car->eval(r);
  ivy_oper *op;
  if ((op = dynamic_cast<ivy_oper*>(oper))) {
    return op->call(cdr, r);
  } else {
    string excp = "Attempted to call " + oper->to_string() +
                  " which is not an operation!";
    throw excp;
  }
}

string ivy_cons::to_string() const {
  string out = "(";
  const ivy_object *elt = this;
  while (!elt->null()) {
    if (const ivy_cons *obj = dynamic_cast<const ivy_cons *>(elt)) {
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
