#include <iostream>       
#include <thread>         

#include "expressions.hpp"
#include "operations.hpp"

using std::list;
using std::string;
using std::thread;

void opcall(hydra_oper* o, hydra_object* args, runtime& r) {
  o->call(args, r);
}

op_thread::op_thread() { eval_args = true; }
hydra_object *op_thread::call(hydra_object *alist, runtime &r) {
  list<hydra_object *> arg_list = get_arg_list(alist, r);

  if (arg_list.size() < 1) {
    string err = "error in thread: expected at least one argument";
    throw err;
  }

  hydra_oper* op = hydra_cast<hydra_oper>(arg_list.first());
  hydra_cons* cns = hydra_cast<hydra_cons>(alist);

  hydra_thread* t = new hydra_thread;
  t.thread = thread(opcall, op, cns->cdr, r);
  return t;
}
