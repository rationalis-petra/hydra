#include <list>

#include "expressions.hpp"
#include "operations.hpp"

using std::list;
using std::string;

using namespace expr;
using type::hydra_cast;

Object *make_list(list<Object *> objects) {
  if (objects.size() == 1) {
    if (dynamic_cast<Cons *>(objects.front()) ||
        (objects.front() == nil::get())) {
      return objects.front();
    } else {
      string err = "Last argument to cons must be of type list!!";
      throw err;
    }
  } else {
    Object *o = objects.front();
    objects.pop_front();
    return new Cons(o, make_list(objects));
  }
}

Object *op_cons(list<Object*> alist, LocalRuntime &r, LexicalScope &s) {
  return make_list(alist);
}

Object *op_cdr(list<Object*> arg_list, LocalRuntime &r, LexicalScope &s) {
  
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

Object *op_car(list<Object*> arg_list, LocalRuntime &r, LexicalScope &s) {
  
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
                        type::Fn::with_all({new type::Any, new type::Any},
                                           new type::Any, new type::Cons),
                        true);

Operator *op::cdr =
    new InbuiltOperator("Takes a cons cell as input, and returns the cdr",
                        op_cdr, type::Fn::with_args({new type::Cons}), true);

Operator *op::car =
    new InbuiltOperator("Takes a cons cell as input, and returns the car",
                        op_car, type::Fn::with_args({new type::Cons}), true);
