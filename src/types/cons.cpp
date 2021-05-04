#include <string>
#include <list>

#include "operations/types.hpp"
#include "expressions.hpp"
#include "types.hpp"

using std::string;

using namespace type;
using namespace interp;

Cons::Cons() {
  type_car = new Any;
  type_cdr = new Any;
  set_invoker(op::mk_cons_type);
}

Cons::Cons(Type* tcar, Type* tcdr) {
  type_car = tcar;
  type_cdr = tcdr;
  set_invoker(op::mk_cons_type);
}

void Cons::mark_node() {
  if (marked) return;
  Object::mark_node();
  type_car->mark_node();
  type_cdr->mark_node();
}

string Cons::to_string(LocalRuntime &r, LexicalScope &s) {
  return "Cons";
}

expr::Object *Cons::check_type(expr::Object *obj) {
  if (expr::Cons *cns = dynamic_cast<expr::Cons *>(obj)) {
    if (type_car->check_type(cns->car)->null()) {
      return expr::nil::get();
    }
    if (type_cdr->check_type(cns->cdr)->null()) {
      return expr::nil::get();
    }
    return expr::t::get();
  } else {
    return expr::nil::get();
  }
}

expr::Object *Cons::subtype(Type *obj) {
  if (Cons *tcons = dynamic_cast<Cons*>(obj)) {
    if (type_car->subtype(tcons->type_car) &&
        type_cdr->subtype(tcons->type_cdr))
    return expr::t::get();
  }
  return expr::nil::get();
}
