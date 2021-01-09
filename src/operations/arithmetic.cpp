#include <list>
#include <string>

#include "expressions.hpp"
#include "operations.hpp"

using std::string;
using std::to_string;
using std::list;

op_plus::op_plus() { eval_args = true; }
hydra_object *op_plus::call(hydra_object *alist, runtime &r) {
  // we guarantee that this is a cons object
  std::list<hydra_object *> arg_list = get_arg_list(alist, r);
  if (arg_list.size() < 2) {
    throw "Insufficient arguments provided to '+' function";
  }
  hydra_num *out = new hydra_num();
  out->value = 0;

  for (hydra_object *o : arg_list) {
    if (hydra_num *num = dynamic_cast<hydra_num *>(o)) {
      out->value += num->value;
    } else {
      string err = "Non-number provided to '+' function: " + o->to_string();
      throw err;
    }
  }
  return out;
}

op_minus::op_minus() { eval_args = true; }
hydra_object *op_minus::call(hydra_object *alist, runtime &r) {
  list<hydra_object *> arg_list = get_arg_list(alist, r);
  if (arg_list.size() < 2) {
    throw "Insufficient arguments provided to '-' function";
  }
  if (hydra_num *out = new hydra_num()) {
    out->value = dynamic_cast<hydra_num *>(arg_list.front())->value;
    arg_list.pop_front();

    for (hydra_object *arg : arg_list) {
      if (hydra_num *num = dynamic_cast<hydra_num *>(arg)) {
        out->value -= num->value;
      } else {
        string err = "Attempt to subtract non-number: " + arg->to_string();
        throw err;
      }
    }
    return out;
  } else {
    string err =
        "Attempt to subtract non-number: " + arg_list.front()->to_string();
    throw err;
  }
}

op_multiply::op_multiply() { eval_args = true; }
hydra_object *op_multiply::call(hydra_object *alist, runtime &r) {
  list<hydra_object *> arg_list = get_arg_list(alist, r);
  if (arg_list.size() < 1) {
    throw "Insufficient arguments provided to '*' function";
  }
  hydra_num *out = new hydra_num();
  out->value = 1;
  arg_list.pop_front();

  for (hydra_object *arg : arg_list) {
    if (hydra_num *num = dynamic_cast<hydra_num *>(arg)) {
      out->value *= num->value;
    } else {
      string err = "Attempt to multiply non-numer: " + arg->to_string();
    }
  }
  return out;
}

op_divide::op_divide() { eval_args = true; }
hydra_object *op_divide::call(hydra_object *alist, runtime &r) {
  list<hydra_object *> arg_list = get_arg_list(alist, r);
  if (arg_list.size() < 1) {
    throw "Insufficient arguments provided to '/' function";
  }
  if (hydra_num *out = new hydra_num()) {
    out->value = dynamic_cast<hydra_num *>(arg_list.front())->value;
    arg_list.pop_front();

    for (hydra_object *arg : arg_list) {
      if (hydra_num *num = dynamic_cast<hydra_num *>(arg)) {
        out->value /= num->value;
      } else {
        string err = "Attempt to divide non-number: " + arg->to_string();
        throw err;
      }
    }
    return out;
  } else {
    string err =
        "Attempt to subtract non-number: " + arg_list.front()->to_string();
    throw err;
  }
}

op_geq::op_geq() { eval_args = true; }
hydra_object *op_geq::call(hydra_object *alist, runtime &r) {
  list<hydra_object *> arg_list = get_arg_list(alist, r);
  if (arg_list.size() != 2) {
    string err = "incorrect number of arguments provided to operator >=: " +
      ::to_string(arg_list.size());
    throw err;
  }
  // we now ASSERT that arg_list is a list of length 2
  // does it contain integers?

  if (hydra_num *a1 = dynamic_cast<hydra_num *>(arg_list.front())) {
    arg_list.pop_front();
    hydra_num *a2 = dynamic_cast<hydra_num *>(arg_list.front());
    // so arg_list is a list containing integers!
    int arg1 = a1->value;
    int arg2 = a2->value;

    if (arg1 >= arg2) {
      return new hydra_t();
    } else {
      return new hydra_nil();
    }
  } else  {
    string err = "Non-numbers provided to >=";
    throw err;
  }
}
