#include "utils.hpp"
#include <list>

#include "expressions.hpp"
#include "operations.hpp"
#include "utils.hpp"

using std::list;
using std::string;

using namespace expr;
using namespace interp;

GenericFn *op::set;
GenericFn *op::get;
GenericFn *op::len;
GenericFn *op::cat;

Operator *op::mk_list;
GenericFn *op::cdr;
GenericFn *op::car;
Operator *op::mk_cons;

Object *internal_cons(list<Object *> objects) {
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
    return new Cons(o, internal_cons(objects));
  }
}

Object *op_mk_list(list<Object *> alist, LocalRuntime &r, LexicalScope &s) {
  alist.push_back(nil::get());
  return internal_cons(alist);
}

Object *op_cons(list<Object *> alist, LocalRuntime &r, LexicalScope &s) {
  return internal_cons(alist);
}

Object *op_cdr(list<Object *> arg_list, LocalRuntime &r, LexicalScope &s) {
  if (Cons *cons = dynamic_cast<Cons *>(arg_list.front())) {
    return cons->cdr;
  } else {
    string err = "Non-cons argument provided to cdr: " +
                 hydra_to_string(arg_list.front(), r, s);
    throw err;
  }
}

Object *op_car(list<Object *> arg_list, LocalRuntime &r, LexicalScope &s) {

  Cons *cons = get_inbuilt<Cons *>(arg_list.front());
  return cons->car;
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

  Operator* in_op_cdr = new InbuiltOperator(
      "cdr", "Takes a cons cell as input, and returns the cdr", op_cdr,
      type::Fn::with_args({new type::Cons}), true);
  op::cdr = new GenericFn;
  op::cdr->type = type::Fn::with_args({new type::Cons});
  op::cdr->add(in_op_cdr);

  Operator* in_op_car = new InbuiltOperator(
      "car", "Takes a cons cell as input, and returns the car", op_car,
      type::Fn::with_args({new type::Cons}), true);
  op::car = new GenericFn;
  op::car->type = type::Fn::with_args({new type::Cons});
  op::car->add(in_op_car);

  Operator* in_cons_eq = new InbuiltOperator(
      "cons =", "Equality test for Conses", op_cons_eq,
      type::Fn::with_args({new type::Cons, new type::Cons}), true);

  op::bin_equal->add(in_cons_eq);

  op::mk_list = new InbuiltOperator(
      "list", "List constructor", op_mk_list,
      type::Fn::with_rest(new type::Any), true);

}
