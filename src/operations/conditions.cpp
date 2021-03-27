#include "expressions.hpp"
#include "operations.hpp"

#include <iostream>

using std::list;
using std::string;

using type::hydra_cast;

using namespace expr;
using namespace interp;

// HANDLING CONDITIONS
Object *op_handler_catch(list<Object *> arg_list, LocalRuntime &r,
                         LexicalScope &s) {

  try {
    // add self to list of handlers
    // LocalRuntime.handlers.add (new catch_handler())
    r.handlers.push_front(new case_handler);
    Object *out = arg_list.front()->eval(r, s);
    for (Object *v : arg_list) {
      Object::collector->remove_root(v);
    }
    r.handlers.pop_front();
    return out;
  }

  catch (hydra_exception *exc) {
    // is this for me?
    if (exc->type != CASE_THROW)
      throw exc;

    r.handlers.pop_front();
    arg_list.pop_front();
    for (Object *o : arg_list) {

      Object *fst = dynamic_cast<Cons *>(o)->car;
      Object *ty = hydra_cast<Cons>(fst)->car;
      type::Type *t = dynamic_cast<type::Type *>(ty->eval(r, s));
      Object::collector->insert_root(t);

      if (!t->check_type(exc->obj)->null()) {
        delete exc;
        Object *out =
            dynamic_cast<Cons *>(dynamic_cast<Cons *>(o)->cdr)->car->eval(r, s);
        Object::collector->remove_root(t);
        for (Object *v : arg_list) {
          Object::collector->remove_root(v);
        }
        return out;
      }
    }
    throw exc;
  }
}

Object *op_handler_bind(list<Object *> arg_list, LocalRuntime &r,
                        LexicalScope &s) {

  // add self to the stack of handlers
  // LocalRuntime.handlers.add(new bind_handler(arg_list))
  Object *code = arg_list.front();
  arg_list.pop_front();
  r.handlers.push_front(new bind_handler(arg_list, r, s));

  try {
    // EXCEPTION THROWN HERRE
    Object *out = code->eval(r, s);

    r.handlers.pop_front();
    for (Object *v : arg_list) {
      Object::collector->remove_root(v);
    }
    return out;
  } catch (hydra_exception *e) {
    // if the exception is a restart, then pop
    if (e->type == RESTART_CALL) {
      r.handlers.pop_front();
    }
    for (Object *v : arg_list) {
      Object::collector->remove_root(v);
    }
    throw e;
  }
}

// RESTARTS

// (with-restart <restart_name> <function> <code>)
Object *op_add_restart(list<Object *> arg_list, LocalRuntime &r,
                       LexicalScope &s) {
  Symbol *sym = dynamic_cast<Symbol *>(arg_list.front()->eval(r, s));
  arg_list.pop_front();
  Operator *op = dynamic_cast<Operator *>(arg_list.front()->eval(r, s));
  arg_list.pop_front();
  hydra_restart *res = new hydra_restart(op, sym);

  r.restarts.push_front(res);

  try {
    Object *ret = arg_list.front()->eval(r, s);

    // NOTE: it seems that the restart wdoesn't get registered until we return??
    r.restarts.pop_front();
    return ret;

  } catch (hydra_exception *ex) {
    if (ex->type == RESTART_CALL && ex->res == res) {
      r.restarts.pop_front();
      LexicalScope s = *(ex->s);
      delete ex->s;
      delete ex;
      return res->op->call(ex->args, r, s);
    } else {
      r.restarts.pop_front();
      throw ex;
    }
  }
}

Object *gen_list(std::list<hydra_restart *> lst) {
  if (lst.empty()) {
    return nil::get();
  } else {
    hydra_restart *r = lst.front();
    lst.pop_front();
    return new Cons(r, gen_list(lst));
  }
}

Object *op_get_restarts(list<Object *> arg_list, LocalRuntime &r,
                        LexicalScope &s) {
  return gen_list(r.restarts);
}

// SIGNALLING CONDITIONS
Object *op_signal_condition(list<Object *> arg_list, LocalRuntime &r,
                            LexicalScope &s) {

  while (!r.handlers.empty()) {
    try {
      return r.handlers.front()->handle(arg_list.front());
    }
    // if this is the case, the condition handler has failed
    catch (bool b) {
      r.handlers.pop_front();
    }
    // if the handler throws an object, then it will get caught by a
    // handler_catch
  }
  // this is a top-level exception!
  hydra_exception *ex = new hydra_exception(CASE_THROW);
  ex->obj = arg_list.front();
  throw ex;
}

Operator *op::handler_catch;
Operator *op::handler_bind;
Operator *op::add_restart;
Operator *op::get_restarts;
Operator *op::signal_condition;

void op::initialize_conditions() {

  op::handler_catch = new InbuiltOperator(
      "hanlder-catch", "Catches a thrown exepction", op_handler_catch,
      type::Fn::with_all({new type::Any}, new type::Cons, new type::Any),
      false);

  op::handler_bind = new InbuiltOperator(
      "handler-bind", "Catches a thrown exepction", op_handler_bind,
      type::Fn::with_all({new type::Any}, new type::Cons, new type::Any),
      false);

  op::add_restart = new InbuiltOperator(
      "add-restart", "signals an object as an exception", op_add_restart,
      type::Fn::with_args({new type::Any, new type::Any, new type::Any}),
      false);

  op::get_restarts = new InbuiltOperator(
      "get-restarts", "Returns the list (stack) of all active restarts!",
      op_get_restarts, type::Fn::with_args({}), true);

  op::signal_condition = new InbuiltOperator(
      "signal-condition", "signals an object as an exception",
      op_signal_condition, type::Fn::with_args({new type::Any}), true);
}
