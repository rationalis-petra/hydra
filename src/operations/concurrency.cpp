#include <iostream>       
#include <thread>         

#include "expressions.hpp"
#include "operations.hpp"

using std::list;
using std::string;

using namespace expr;

Object *op_mk_thread(list<Object*> arg_list, LocalRuntime &r, LexicalScope& s) {
  Operator* op = dynamic_cast<Operator*>(arg_list.front());

  Thread *t = new Thread(op, r, s);
  //t->thread = thread(opcall, op, cns->cdr, r);
  return t;
}

Operator *op::thread =
  new InbuiltOperator ("Takes function Fn () -> Any and creates a new thread"
                       "\n where it calls that function",
                       op_mk_thread,
                       type::Fn::with_args({type::Fn::with_args({})}),
                       true);
