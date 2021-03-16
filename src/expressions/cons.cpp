#include <string>
#include <typeinfo>

#include "expressions.hpp"

using namespace std;
using namespace expr;

void Cons::mark_node() {
  if (marked) return;
  Object::mark_node();
  car->mark_node();
  cdr->mark_node();
}

Cons::Cons(Object *_car, Object *_cdr)
    : car(_car), cdr(_cdr) {
}

Object *Cons::eval(LocalRuntime &r, LexicalScope &s) {
  Object *oper = car->eval(r, s);

  Object::roots.insert(oper);

  Operator *op;
  if ((op = dynamic_cast<Operator *>(oper))) {
    try {
      std::list<Object*> arg_list = op->get_arg_list(cdr, r, s);
      Object *out = op->call(arg_list, r, s);
      Object::roots.remove(oper);
      return out;
    } catch (hydra_exception* e) {
      roots.remove(oper);
      throw e;
    }
  } else if (oper->invoker) {
    try {
      std::list<Object*> arg_list = oper->invoker->get_arg_list(cdr, r, s);
      Object *out = oper->invoker->call(arg_list, r, s);
      Object::roots.remove(oper);
      return out;
    } catch (hydra_exception* e) {
      roots.remove(oper);
      throw e;
    }
  } else {
    string excp = "Attempted to call " + oper->to_string() +
                  " which is not an operation!";
    roots.remove(oper);
    throw excp;
  }
}

string Cons::to_string() const {
  string out = "(";
  const Object *elt = this;
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

