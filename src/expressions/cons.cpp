#include <string>
#include <typeinfo>
#include <iostream>

#include "expressions.hpp"

using namespace std;
using namespace expr;

void Cons::mark_node() {
  if (marked) return;
  marked = true;
  car->mark_node();
  cdr->mark_node();
}

Cons::Cons(Value *_car, Value *_cdr)
    : car(_car), cdr(_cdr) {
}

Value *Cons::eval(LocalRuntime &r, LexicalScope &s) {
  Value *oper = car->eval(r, s);
  Value::roots.insert(oper);
  Operator *op;
  if ((op = dynamic_cast<Operator *>(oper))) {
    try {
      Value *out = op->call(cdr, r, s);
      Value::roots.remove(oper);
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

string Cons::to_string() const {
  string out = "(";
  const Value *elt = this;
  while (!elt->null()) {
    if (const Cons *obj = dynamic_cast<const Cons *>(elt)) {
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

