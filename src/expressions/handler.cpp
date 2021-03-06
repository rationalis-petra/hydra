#include "expressions.hpp"
#include "utils.hpp"

using namespace expr;
using namespace interp;

// HANDLER BIND
bind_handler::bind_handler(std::list<Object *> lst, LocalRuntime &run,
                           LexicalScope &sc)
    : r(run), s(sc) {
  handling_code = lst;
}

Object *bind_handler::handle(Object *condition) {
  for (Object *o : handling_code) {
    Object *fst = get_inbuilt<Cons *>(o)->car;
    Object *ty = get_inbuilt<Cons*>(fst)->car;
    type::Type *t = get_inbuilt<type::Type *>(ty->eval(r, s));

    if (!t->check_type(condition)->null()) {
      return get_inbuilt<Cons *>(get_inbuilt<Cons *>(o)->cdr)
          ->car->eval(r, s);
    }
  }
  // indicate that the handler failed
  throw false;
}

Object *case_handler::handle(Object *condition) {
  hydra_exception *ex = new hydra_exception(CASE_THROW);
  ex->obj = condition;
  throw ex;
}


// A RESTART

hydra_restart::hydra_restart(Operator *o, Symbol *s) : op(o), sym(s) {}

void hydra_restart::mark_node() {
  if (marked) return;
  Object::mark_node();
  op->mark_node();
  sym->mark_node();
}

Object *hydra_restart::call(std::list<Object*> arg_list, LocalRuntime &r, LexicalScope &s, bool) {
  // invokesn a restart
  hydra_exception *e = new hydra_exception(RESTART_CALL);
  e->res = this;
  e->args = arg_list;
  e->s = new LexicalScope(s);
  throw e;
}

hydra_exception::hydra_exception(exception_type _type) : type(_type) {}
