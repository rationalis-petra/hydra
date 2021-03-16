#include <iostream>       
#include <thread>         

#include "expressions.hpp"
#include "operations.hpp"

using std::list;
using std::string;

using namespace expr;

Object *op_mk_thread(list<Object*> arg_list, LocalRuntime &r, LexicalScope& s) {
  Operator* op = dynamic_cast<Operator*>(arg_list.front());
  arg_list.pop_front();

  Thread *t = new Thread(op, arg_list, r, s);
  return t;
}

Operator *op::thread =
  new InbuiltOperator ("Takes function Fn () -> Any and creates a new thread"
                       "\n where it calls that function",
                       op_mk_thread,
                       // TODO: introduce type Thread...
                       type::Fn::with_all({type::Fn::with_rest(new type::Any)}, new type::Any, new type::Any),
                       true);
