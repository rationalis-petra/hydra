#include <string>
#include <typeinfo>

#include "expressions.hpp"
#include "utils.hpp"

using namespace std;
using namespace expr;

Parent* Cons::parent;

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

  if (oper->invoker) {
    try {
      Operator* op = oper->invoker;
      std::list<Object*> arg_list = op->get_arg_list(cdr, r, s);
      Object *out = op->call(arg_list, r, s);
      Object::roots.remove(oper);
      return out;
    } catch (hydra_exception* e) {
      roots.remove(oper);
      throw e;
    }
  } else {
    string excp = "Attempted to call " + hydra_to_string(oper, r, s) +
                  " which is not an operation!";
    roots.remove(oper);
    throw excp;
  }
}

string Cons::to_string(LocalRuntime &r, LexicalScope &s) {
  string out = "(";
  const Object *elt = this;
  while (!elt->null()) {
    if (const Cons *obj = dynamic_cast<const Cons *>(elt)) {
      out += hydra_to_string(obj->car, r, s);
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

