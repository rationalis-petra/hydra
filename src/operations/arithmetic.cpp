#include <list>
#include <string>

#include "expressions.hpp"
#include "operations.hpp"

using std::bad_cast;
using std::string;
using std::to_string;
using std::list;

op_plus::op_plus() { eval_args = true; }
ivy_object *op_plus::call(ivy_object *alist, runtime &r) {
  // we guarantee that this is a cons object
  std::list<ivy_object *> arg_list = get_arg_list(alist, r);
  if (arg_list.size() < 2) {
    throw "Insufficient arguments provided to '+' function";
  }
  ivy_num *out = new ivy_num();
  out->val = 0;

  for (ivy_object *o : arg_list) {
    if (ivy_num *num = dynamic_cast<ivy_num *>(o)) {
      out->val += num->val;
    } else {
      string err = "Non-number provided to '+' function: " + o->to_string();
      throw err;
    }
  }
  return out;
}

op_minus::op_minus() { eval_args = true; }
ivy_object *op_minus::call(ivy_object *alist, runtime &r) {
  list<ivy_object *> arg_list = get_arg_list(alist, r);
  if (arg_list.size() < 2) {
    throw "Insufficient arguments provided to '-' function";
  }
  try {
    ivy_num *out = new ivy_num();
    out->val = dynamic_cast<ivy_num *>(arg_list.front())->val;
    arg_list.pop_front();

    for (ivy_object *arg : arg_list) {
      try {
        ivy_num *num = dynamic_cast<ivy_num *>(arg);
        out->val -= num->val;
      } catch (std::bad_cast&) {
        string err = "Attempt to subtract non-number: " + arg->to_string();
        throw err;
      }
    }
    return out;
  } catch (bad_cast&) {
    string err =
        "Attempt to subtract non-number: " + arg_list.front()->to_string();
    throw err;
  }
}

op_multiply::op_multiply() { eval_args = true; }
ivy_object *op_multiply::call(ivy_object *alist, runtime &r) {
  list<ivy_object *> arg_list = get_arg_list(alist, r);
  if (arg_list.size() < 1) {
    throw "Insufficient arguments provided to '*' function";
  }
  ivy_num *out = new ivy_num();
  out->val = 1;
  arg_list.pop_front();

  for (ivy_object *arg : arg_list) {
    try {
      ivy_num *num = dynamic_cast<ivy_num *>(arg);
      out->val *= num->val;
    } catch (bad_cast&) {
      string err = "Attempt to multiply non-numer: " + arg->to_string();
    }
  }
  return out;
}

op_divide::op_divide() { eval_args = true; }
ivy_object *op_divide::call(ivy_object *alist, runtime &r) {
  list<ivy_object *> arg_list = get_arg_list(alist, r);
  if (arg_list.size() < 1) {
    throw "Insufficient arguments provided to '/' function";
  }
  try {
    ivy_num *out = new ivy_num();
    out->val = dynamic_cast<ivy_num *>(arg_list.front())->val;
    arg_list.pop_front();

    for (ivy_object *arg : arg_list) {
      try {
        ivy_num *num = dynamic_cast<ivy_num *>(arg);
        out->val /= num->val;
      } catch (bad_cast&) {
        string err = "Attempt to divide non-number: " + arg->to_string();
        throw err;
      }
    }
    return out;
  } catch (bad_cast&) {
    string err =
        "Attempt to subtract non-number: " + arg_list.front()->to_string();
    throw err;
  }
}

op_geq::op_geq() { eval_args = true; }
ivy_object *op_geq::call(ivy_object *alist, runtime &r) {
  list<ivy_object *> arg_list = get_arg_list(alist, r);
  if (arg_list.size() != 2) {
    string err = "incorrect number of arguments provided to operator >=: " +
      ::to_string(arg_list.size());
    throw err;
  }
  // we now ASSERT that arg_list is a list of length 2
  // does it contain integers?

  try {
    ivy_num *a1 = dynamic_cast<ivy_num *>(arg_list.front());
    arg_list.pop_front();
    ivy_num *a2 = dynamic_cast<ivy_num *>(arg_list.front());
    // so arg_list is a list containing integers!
    int arg1 = a1->val;
    int arg2 = a2->val;

    if (arg1 >= arg2) {
      return new ivy_t();
    } else {
      return new ivy_nil();
    }
  } catch (bad_cast&) {
    string err = "Non-numbers provided to >=";
    throw err;
  }
}
