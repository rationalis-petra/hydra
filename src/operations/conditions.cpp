#include "expressions.hpp"
#include "operations.hpp"

#include <iostream>

using std::list;
using std::string;

// HANDLING CONDITIONS
op_handler_catch::op_handler_catch() {
  is_fn = false;
  docstring = new hydra_string("Catches a thrown exepction");
  type->arg_list.push_front(new type_nil);
  type->rest_type = new type_cons;
}

hydra_object* op_handler_catch::call(hydra_object* alist, runtime &r, lexical_scope& s) {
  list<hydra_object*> arg_list = get_arg_list(alist, r, s);
  try {
    // add self to list of handlers
    // runtime.handlers.add (new catch_handler())
    r.handlers.push_front(new case_handler);
    hydra_object* out = arg_list.front()->eval(r, s);
    r.handlers.pop_front();
    return out;
  }
  catch (hydra_exception* exc) {
    // is this for me?
    if (exc->type != CASE_THROW)
      throw exc;

    r.handlers.pop_front();
    arg_list.pop_front();
    for (hydra_object *o : arg_list) {

      hydra_object* fst = dynamic_cast<hydra_cons*>(o)->car;
      hydra_object* ty = hydra_cast<hydra_cons>(fst)->car;
      hydra_type *t = dynamic_cast<hydra_type*>(ty->eval(r, s));

      if (!t->check_type(exc->obj)->null()) {
        delete exc;
        return dynamic_cast<hydra_cons *>(dynamic_cast<hydra_cons *>(o)->cdr)
            ->car->eval(r, s);
      }
    }
    throw exc;
  }
}

op_handler_bind::op_handler_bind() {
  is_fn = false;
  docstring = new hydra_string("Catches a thrown exepction");
  type->arg_list.push_front(new type_nil);
  type->rest_type = new type_cons;
}


hydra_object* op_handler_bind::call(hydra_object* alist, runtime &r, lexical_scope& s) {
  list<hydra_object*> arg_list = get_arg_list(alist, r, s);
  // add self to the stack of handlers
  // runtime.handlers.add(new bind_handler(arg_list))
  hydra_object *code = arg_list.front();
  arg_list.pop_front();
  r.handlers.push_front(new bind_handler(arg_list, r, s));
  hydra_object *out = code->eval(r, s);
  r.handlers.pop_front();
  return out;
}

// RESTARTS
op_add_restart::op_add_restart () {
  is_fn = true;
  docstring = new hydra_string("signals an object as an exception");
  type->arg_list.push_front(new type_symbol);
  type->arg_list.push_front(new type_fn);
  type->arg_list.push_front(new type_nil);
}

// (with-restart <restart_name> <function> <code>)
hydra_object* op_add_restart::call(hydra_object* alist, runtime &r, lexical_scope& s) {
  list<hydra_object *> arg_list = get_arg_list(alist, r, s);

  hydra_symbol* sym = dynamic_cast<hydra_symbol*> (arg_list.front());
  arg_list.pop_front();
  hydra_oper* op = dynamic_cast<hydra_oper*>(sym);
  arg_list.pop_front();
  hydra_restart* res = new hydra_restart(op, sym);

  r.restarts.push_front(res);
  // TODO: find 
  try {
    return arg_list.front()->eval(r, s);
  } catch (hydra_exception* ex) {
    if (ex->type == RESTART_CALL && ex->res == res) {
      lexical_scope s = *(ex->s);
      delete ex->s;
      delete ex;
      return res->op->call(ex->args, r, s);
    } else {
      throw ex;
    }
  }
}

op_get_restarts::op_get_restarts () {
  is_fn = true;
  docstring =
      new hydra_string("Returns the list (stack) of all active restarts!");
}

hydra_object* gen_list(std::list<hydra_restart*> lst) {
  if (lst.empty()) {
    return hydra_nil::get();
  } else {
    return new hydra_cons(lst.front(), (lst.pop_front(), gen_list(lst)));
  }
}
hydra_object *op_get_restarts::call(hydra_object *alist, runtime &r,
                                    lexical_scope &s) {
  return gen_list(r.restarts);
}


// SIGNALLING CONDITIONS
op_signal_condition::op_signal_condition() {
  is_fn = true;
  docstring = new hydra_string("signals an object as an exception");
  type->arg_list.push_front(new type_nil);
}


hydra_object* op_signal_condition::call(hydra_object* alist, runtime &r, lexical_scope& s) {
  list<hydra_object*> arg_list = get_arg_list(alist, r, s);
  while (!r.handlers.empty()) {
    try {
      return r.handlers.front()->handle(arg_list.front());
    }
    // if this is the case, the condition handler has failed
    catch (bool b)  {
      r.handlers.pop_front();
    }
    // if the handler throws an object, then it will get caught by a handler_catch
  }
  // this is a top-level exception!
  hydra_exception* ex = new hydra_exception(CASE_THROW);
  ex->obj = arg_list.front();
  throw ex;
}
