#include <list>

#include "expressions.hpp"
#include "operations.hpp"

using std::list;
using std::string;

op_cons::op_cons() {
  is_fn = true;
  docstring = new hydra_string("Creates a new cons cell and places the first "
                               "argument in the car,\n the second in the cdr."
                               "Will fail if the second is not cons or nil");
  type->arg_list.push_back(new type_nil);
  type->arg_list.push_back(new type_list);
}
hydra_object *op_cons::call(hydra_object *alist, runtime &r, lexical_scope &s) {
  list<hydra_object *> arg_list = get_arg_list(alist, r, s);
  if (arg_list.size() != 2) {
    throw "invalid number of args to cons";
  }
  hydra_object *car = arg_list.front();
  arg_list.pop_front();
  hydra_object *cdr = arg_list.front();

  if (!(dynamic_cast<hydra_cons *>(cdr) || dynamic_cast<hydra_nil *>(cdr))) {
    string err =
        "Non-list provided as second argument to cons: " + cdr->to_string();
    throw err;
  }

  hydra_cons *out = new hydra_cons(car, cdr);
  return out;
}

op_cdr::op_cdr() {
  is_fn = true;
  docstring =
      new hydra_string("Takes a cons cell as input, and returns the cdr");
  type->arg_list.push_front(new type_cons);
}
hydra_object *op_cdr::call(hydra_object *alist, runtime &r, lexical_scope &s) {
  list<hydra_object *> arg_list = get_arg_list(alist, r, s);
  if (arg_list.size() != 1) {
    throw "invalid number of args to cdr";
  }

  if (hydra_cons *cons = dynamic_cast<hydra_cons *>(arg_list.front())) {
    return cons->cdr;
  } else {
    string err =
        "Non-cons argument provided to cdr: " + arg_list.front()->to_string();
    throw err;
  }
}

op_car::op_car() {
  is_fn = true;
  docstring =
      new hydra_string("Takes a cons cell as input, and returns the car");
  type->arg_list.push_front(new type_cons);
}
hydra_object *op_car::call(hydra_object *alist, runtime &r, lexical_scope &s) {
  list<hydra_object *> arg_list = get_arg_list(alist, r, s);
  if (arg_list.size() != 1) {
    throw "invalid number of args to car";
  }

  if (hydra_cons *cons = dynamic_cast<hydra_cons *>(arg_list.front())) {
    return cons->car;
  } else {
    string err =
        "Non-cons argument provided to car: " + arg_list.front()->to_string();
    throw err;
  }
}



