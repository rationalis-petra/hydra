#include <list>

#include "expressions.hpp"
#include "operations.hpp"

using std::list;
using std::string;

using namespace expr;
using type::hydra_cast;

Value *make_list(list<Value *> objects) {
  if (objects.size() == 1) {
    if (dynamic_cast<Cons *>(objects.front()) ||
        (objects.front() == nil::get())) {
      return objects.front();
    } else {
      string err = "Last argument to cons must be of type list!!";
      throw err;
    }
  } else {
    Value *o = objects.front();
    objects.pop_front();
    return new Cons(o, make_list(objects));
  }
}

Value *op_cons(Operator *op, Value *alist, LocalRuntime &r, LexicalScope &s) {
  list<Value *> arg_list = op->get_arg_list(alist, r, s);

  return make_list(arg_list);
}

Value *op_cdr(Operator *op, Value *alist, LocalRuntime &r, LexicalScope &s) {
  list<Value *> arg_list = op->get_arg_list(alist, r, s);
  if (arg_list.size() != 1) {
    throw "invalid number of args to cdr";
  }

  if (Cons *cons = dynamic_cast<Cons *>(arg_list.front())) {
    return cons->cdr;
  } else {
    string err =
        "Non-cons argument provided to cdr: " + arg_list.front()->to_string();
    throw err;
  }
}

Value *op_car(Operator* op, Value *alist, LocalRuntime &r, LexicalScope &s) {
  list<Value *> arg_list = op->get_arg_list(alist, r, s);
  if (arg_list.size() != 1) {
    throw "invalid number of args to car";
  }

  if (Cons *cons = dynamic_cast<Cons *>(arg_list.front())) {
    return cons->car;
  } else {
    string err =
        "Non-cons argument provided to car: " + arg_list.front()->to_string();
    throw err;
  }
}

Operator* op::mk_cons =
    new InbuiltOperator("Creates a new cons cell and places the first "
                        "argument in the car,\n the second in the cdr."
                        "Will fail if the second is not cons or nil",
                        op_cons,
                        type::Fn::with_all({new type::Nil, new type::Nil},
                                           new type::Nil, new type::Cons),
                        true);

Operator *op::cdr =
    new InbuiltOperator("Takes a cons cell as input, and returns the cdr",
                        op_cdr, type::Fn::with_args({new type::Cons}), true);

Operator *op::car =
    new InbuiltOperator("Takes a cons cell as input, and returns the car",
                        op_car, type::Fn::with_args({new type::Cons}), true);
