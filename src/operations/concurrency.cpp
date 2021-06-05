#include <iostream>       
#include <thread>         

#include "expressions.hpp"
#include "operations.hpp"
#include "utils.hpp"

using std::list;
using std::string;

using namespace expr;
using namespace interp;

Object *op_mk_thread(list<Object*> arg_list, LocalRuntime &r, LexicalScope& s) {
  Operator* op = dynamic_cast<Operator*>(arg_list.front());
  arg_list.pop_front();

  Thread *t = new Thread(op, arg_list, r, s);
  return t;
}

Object *op_mk_mutex(list<Object*> arg_list, LocalRuntime &r, LexicalScope& s) {
  return new Mutex;
}

Object *op_lock_mutex(list<Object*> arg_list, LocalRuntime &r, LexicalScope& s) {
  Mutex* mtx = get_inbuilt<Mutex*>(arg_list.front());
  mtx->lock();
  return mtx;
}

Object *op_unlock_mutex(list<Object*> arg_list, LocalRuntime &r, LexicalScope& s) {
  Mutex* mtx = get_inbuilt<Mutex*>(arg_list.back());
  mtx->unlock();
  return mtx;
}


Operator *op::thread;
Operator *op::mk_mutex;
GenericFn *op::lock_mutex;
GenericFn *op::unlock_mutex;

void op::initialize_concurrency() {

  op::thread = new InbuiltOperator(
                                   "thread",
      "Takes function Fn () -> Any and creates a new thread"
      "\n where it calls that function",
      op_mk_thread,
      // TODO: introduce type Thread...
      type::Fn::with_all({type::Fn::with_rest(new type::Any)}, new type::Any,
                         new type::Any),
      true);

  op::mk_mutex
    = new InbuiltOperator("mutex",
                          "Creates a new mutual-exclusion lock",
                          op_mk_mutex,
                          type::Fn::with_args({}),
                          true);
  Operator* in_op_lock =
    new InbuiltOperator("lock",
                        "Locks a mutual-exclusion lock",
                        op_lock_mutex,
                        type::Fn::with_args({type::mutex_type}),
                        true);
  op::lock_mutex = new GenericFn("lock");
  op::lock_mutex->type = type::Fn::with_args({type::mutex_type});
  op::lock_mutex->add(in_op_lock);
  
  Operator* in_op_unlock =
    new InbuiltOperator("lock",
                        "Locks a mutual-exclusion lock",
                        op_unlock_mutex,
                        type::Fn::with_args({type::mutex_type}),
                        true);
  op::unlock_mutex = new GenericFn("unlock");
  op::unlock_mutex->type = type::Fn::with_args({type::mutex_type});
  op::unlock_mutex->add(in_op_unlock);
}
