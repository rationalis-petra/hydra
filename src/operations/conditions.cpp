#include "expressions.hpp"
#include "operations.hpp"

#include <iostream>

using std::list;
using std::string;

using type::hydra_cast;

using namespace expr;

// HANDLING CONDITIONS
Value* op_handler_catch(Operator* op, Value* alist, LocalRuntime &r, LexicalScope& s) {
  list<Value*> arg_list = op->get_arg_list(alist, r, s);
  try {
    // add self to list of handlers
    // LocalRuntime.handlers.add (new catch_handler())
    r.handlers.push_front(new case_handler);
    Value* out = arg_list.front()->eval(r, s);
    for (Value* v : arg_list) {
      Value::roots.remove(v);
    }
    r.handlers.pop_front();
    return out;
  }

  catch (hydra_exception* exc) {
    // is this for me?
    if (exc->type != CASE_THROW)
      throw exc;

    r.handlers.pop_front();
    arg_list.pop_front();
    for (Value *o : arg_list) {

      Value* fst = dynamic_cast<Cons*>(o)->car;
      Value* ty = hydra_cast<Cons>(fst)->car;
      type::Type *t = dynamic_cast<type::Type*>(ty->eval(r, s));
      Value::roots.insert(t);

      if (!t->check_type(exc->obj)->null()) {
        delete exc;
        Value* out =  dynamic_cast<Cons *>(dynamic_cast<Cons *>(o)->cdr)
            ->car->eval(r, s);
        Value::roots.remove(t);
        for (Value *v : arg_list) {
          Value::roots.remove(v);
        }
        return out;
      }
    }
    throw exc;
  }
}

Operator* op::handler_catch =
  new InbuiltOperator("Catches a thrown exepction",
                      op_handler_catch, type::Fn::with_all({new type::Any}, new type::Cons, new type::Any),
                      false);

Value* op_handler_bind(Operator* op, Value* alist, LocalRuntime &r, LexicalScope& s) {
  list<Value*> arg_list = op->get_arg_list(alist, r, s);
  // add self to the stack of handlers
  // LocalRuntime.handlers.add(new bind_handler(arg_list))
  Value *code = arg_list.front();
  arg_list.pop_front();
  r.handlers.push_front(new bind_handler(arg_list, r, s));


  try {
    // EXCEPTION THROWN HERRE
    Value *out = code->eval(r, s);


    r.handlers.pop_front();
    for (Value *v : arg_list) {
      Value::roots.remove(v);
    }
    return out;
  } catch (hydra_exception* e) {
    // if the exception is a restart, then pop
    if (e->type == RESTART_CALL) {
      r.handlers.pop_front();
    }
    for (Value *v : arg_list) {
      Value::roots.remove(v);
    }
    throw e;
  }
}

Operator* op::handler_bind =
  new InbuiltOperator("Catches a thrown exepction",
                      op_handler_bind,
                      type::Fn::with_all({new type::Any}, new type::Cons, new type::Any),false);

// RESTARTS

// (with-restart <restart_name> <function> <code>)
Value* op_add_restart(Operator* op2, Value* alist, LocalRuntime &r, LexicalScope& s) {
  list<Value *> arg_list = op2->get_arg_list(alist, r, s);

  Symbol* sym = dynamic_cast<Symbol*> (arg_list.front()->eval(r, s));
  arg_list.pop_front();
  Operator* op = dynamic_cast<Operator*>(arg_list.front()->eval(r, s));
  arg_list.pop_front();
  hydra_restart* res = new hydra_restart(op, sym);

  r.restarts.push_front(res);

  try {
    Value* ret = arg_list.front()->eval(r, s);

    // NOTE: it seems that the restart wdoesn't get registered until we return??
    r.restarts.pop_front();
    return ret;

  } catch (hydra_exception* ex) {
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

Operator* op::add_restart =
  new InbuiltOperator("signals an object as an exception",
                      op_add_restart,
                      type::Fn::with_args({new type::Any, new type::Any, new type::Any}),
                      false);

Value* gen_list(std::list<hydra_restart*> lst) {
  if (lst.empty()) {
    return nil::get();
  } else {
    hydra_restart* r = lst.front();
    lst.pop_front();
    return new Cons(r, gen_list(lst));
  }
}

Value *op_get_restarts(Operator *op, Value *alist, LocalRuntime &r,
                       LexicalScope &s) {
  return gen_list(r.restarts);
}

Operator* op::get_restarts =
  new InbuiltOperator("Returns the list (stack) of all active restarts!",
                      op_get_restarts,
                      type::Fn::with_args({}),
                      true);

// SIGNALLING CONDITIONS
Value* op_signal_condition(Operator* op, Value* alist, LocalRuntime &r, LexicalScope& s) {
  list<Value*> arg_list = op->get_arg_list(alist, r, s);
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


Operator* op::signal_condition =
  new InbuiltOperator("signals an object as an exception",
                      op_signal_condition,
                      type::Fn::with_args({new type::Any}),
                      true);
