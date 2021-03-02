#include "expressions.hpp"

bind_handler::bind_handler(std::list<hydra_object *> lst, runtime &run,
                           lexical_scope &sc)
    : r(run), s(sc) {
  handling_code = lst;
}

hydra_object* bind_handler::handle(hydra_object* condition) {

  for (hydra_object *o : handling_code) {
    hydra_object *fst = dynamic_cast<hydra_cons *>(o)->car;
    hydra_object *ty = hydra_cast<hydra_cons>(fst)->car;
    hydra_type *t = dynamic_cast<hydra_type *>(ty->eval(r, s));

    if (!t->check_type(condition)->null()) {
      return dynamic_cast<hydra_cons *>(dynamic_cast<hydra_cons *>(o)->cdr)
          ->car->eval(r, s);
    }
  }
  // indicate that the handler failed
  throw false;
}

hydra_object* case_handler::handle(hydra_object* condition) {
  hydra_exception* ex = new hydra_exception(CASE_THROW);
  ex->obj = condition;
  throw ex;
}

hydra_restart::hydra_restart(hydra_oper* o, hydra_symbol *s) : op(o), sym(s) {}

hydra_object *hydra_restart::call(hydra_object *arg_list, runtime &r,
                                  lexical_scope &s) {
  // invokesn a restart
  hydra_exception* e = new hydra_exception(RESTART_CALL);
  e->res = this;
  e->args = arg_list;
  e->s = new lexical_scope(s);
  throw e;
}

hydra_exception::hydra_exception(exception_type _type) : type(_type) {}

