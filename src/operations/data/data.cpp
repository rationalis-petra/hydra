#include <list>

#include "expressions.hpp"
#include "operations.hpp"

using std::list;
using std::string;

hydra_object* make_list(list<hydra_object*> objects) {
  if (objects.size() == 1) {
    if (dynamic_cast<hydra_cons*>(objects.front()) || (objects.front() == hydra_nil::get())) {
      return objects.front();
    } else {
      string err = "Last argument to cons must be of type list!!";
      throw err;
    }
  } else {
    hydra_object* o = objects.front();
    objects.pop_front();
    return new hydra_cons(o, make_list(objects));
  }
}

op_cons::op_cons() {
  is_fn = true;
  docstring = new hydra_string("Creates a new cons cell and places the first "
                               "argument in the car,\n the second in the cdr."
                               "Will fail if the second is not cons or nil");
  type->arg_list.push_back(new type_nil);
  type->arg_list.push_back(new type_nil);
  type->rest_type = new type_nil;
  type->return_type = new type_cons;
}
hydra_object *op_cons::call(hydra_object *alist, runtime &r, lexical_scope &s) {
  list<hydra_object *> arg_list = get_arg_list(alist, r, s);

  return make_list(arg_list);
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



