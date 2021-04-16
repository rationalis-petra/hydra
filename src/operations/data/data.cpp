#include "utils.hpp"
#include <list>

#include "expressions.hpp"
#include "operations.hpp"

using std::list;
using std::string;

using namespace expr;
using namespace interp;

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

Object *op_cons(list<Object *> alist, LocalRuntime &r, LexicalScope &s) {
  return make_list(alist);
}

Object *op_cdr(list<Object *> arg_list, LocalRuntime &r, LexicalScope &s) {

  if (arg_list.size() != 1) {
    throw "invalid number of args to cdr";
  }

  if (Cons *cons = dynamic_cast<Cons *>(arg_list.front())) {
    return cons->cdr;
  } else {
    string err = "Non-cons argument provided to cdr: " +
                 hydra_to_string(arg_list.front(), r, s);
    throw err;
  }
}

Object *op_car(list<Object *> arg_list, LocalRuntime &r, LexicalScope &s) {

  if (arg_list.size() != 1) {
    throw "invalid number of args to car";
  }

  if (Cons *cons = dynamic_cast<Cons *>(arg_list.front())) {
    return cons->car;
  } else {
    string err = "Non-cons argument provided to car: " +
                 hydra_to_string(arg_list.front(), r, s);
    throw err;
  }
}

Object *op_cons_eq(list<Object *> arg_list, LocalRuntime &r, LexicalScope &s) {

  Cons *cons1 = dynamic_cast<Cons *>(arg_list.front());
  Cons *cons2 = dynamic_cast<Cons *>(arg_list.back());

  if (equal_operator->call({cons1->car, cons2->car}, r, s)->null()) {
    return nil::get();
  } else if (equal_operator->call({cons1->cdr, cons2->cdr}, r, s)->null()) {
    return nil::get();
  } else {
    return t::get();
  }
}

GenericFn *op::set;
GenericFn *op::get;
GenericFn *op::len;
GenericFn *op::cat;
//GenericFn *op::len;

Operator *op::mk_cons;
Operator *op::cdr;
Operator *op::car;


void op::initialize_data() {
  op::set = new GenericFn();
  op::set->type = type::Fn::with_all({new type::Any, new type::Any},
                                     new type::Any, new type::Any);
  
  op::get = new GenericFn();
  op::get->type = type::Fn::with_all({new type::Any},
                                     new type::Any, new type::Any);

  op::len = new GenericFn();
  op::len->type = type::Fn::with_args({new type::Any});

  op::cat = new GenericFn();
  op::cat->type = type::Fn::with_rest(new type::Any);
}

void op::initialize_cons() {


  op::mk_cons =
      new InbuiltOperator("cons",
                          "Creates a new cons cell and places the first "
                          "argument in the car,\n the second in the cdr."
                          "Will fail if the second is not cons or nil",
                          op_cons,
                          type::Fn::with_all({new type::Any, new type::Any},
                                             new type::Any, new type::Cons),
                          true);
  op::cdr = new InbuiltOperator(
      "cdr", "Takes a cons cell as input, and returns the cdr", op_cdr,
      type::Fn::with_args({new type::Cons}), true);

  op::car = new InbuiltOperator(
      "car", "Takes a cons cell as input, and returns the car", op_car,
      type::Fn::with_args({new type::Cons}), true);
  Operator* in_cons_eq = new InbuiltOperator(
      "cons =", "Equality test for Conses", op_cons_eq,
      type::Fn::with_args({new type::Cons, new type::Cons}), true);

  op::bin_equal->add(in_cons_eq);

}
