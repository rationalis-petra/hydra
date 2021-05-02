#include <string>
#include <typeinfo>

#include "utils.hpp"
#include "expressions.hpp"

using namespace std;
using namespace expr;
using namespace interp;

Parent* Cons::parent;

Cons::Cons(Object *_car, Object *_cdr)
    : car(_car), cdr(_cdr) {
}

void Cons::mark_node() {
  if (marked) return;
  Object::mark_node();
  car->mark_node();
  cdr->mark_node();
}

Object* Cons::eval(LocalRuntime &r, LexicalScope &s) {
  Object *oper = car->eval(r, s);
  collector->insert_root(oper);

  if (oper->invoker) {
    try {
      Operator* op = oper->invoker;
      std::list<Object*> arg_list = op->get_arg_list(cdr, r, s);
      Object* out = op->call(arg_list, r, s);
      collector->remove_root(oper);
      return out;
    } catch (hydra_exception* e) {
      collector->remove_root(oper);
      throw e;
    }
  } else if (Operator* op = get_inbuilt<Operator*>(oper)) {
    try {
      std::list<Object*> arg_list = op->get_arg_list(cdr, r, s);
      Object* out = op->call(arg_list, r, s);
      collector->remove_root(oper);
      return out;
    } catch (hydra_exception* e) {
      collector->remove_root(oper);
      throw e;
    }
  } else {
    string err = "car is not an operator";
    throw err;
  }
}

string Cons::to_string(LocalRuntime &r, LexicalScope& s) {
  string out = "(";
  Object *elt = this;
  while (!elt->null()) {
    if (Cons *obj = dynamic_cast<Cons *>(elt)) {
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
