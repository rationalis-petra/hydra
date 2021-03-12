#include <iostream>       
#include <thread>         

#include "expressions.hpp"
#include "operations.hpp"

using std::list;
using std::string;
using std::thread;

using type::hydra_cast;

using namespace expr;

// Value *op_thread(Operator *op, Value *alist, Runtime &r) {
//   list<Value *> arg_list = op->get_arg_list(alist, r);

//   if (arg_list.size() < 1) {
//     string err = "error in thread: expected at least one argument";
//     throw err;
//   }

//   Operator* op = hydra_cast<Operator>(arg_list.first());
//   Cons* cns = hydra_cast<Cons>(alist);

//   Thread* t = new Thread;
//   t.thread = thread(opcall, op, cns->cdr, r);
//   return t;
// }

// Operator* op::thread = new InbuiltOperator*("Creates a new thread and starts the provided function within that thread",
//                                             op_thread, nullptr, true);
