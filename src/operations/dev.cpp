#include <chrono>
#include <list>
#include <time.h>

#include "expressions.hpp"
#include "operations.hpp"
#include "utils.hpp"

using namespace std::chrono;
using namespace interp;
using namespace expr;

using std::list;
using std::string;

Object *op_describe(list<Object *> arg_list, LocalRuntime &r, LexicalScope &s) {

  if (UserOperator *uop = dynamic_cast<UserOperator *>(arg_list.front())) {
    string str =
        "generic function:\n" + uop->docstring->value +
        "\nlambda-list: " + "\ntype: " + hydra_to_string(uop->type, r, s);
    // str += "\n expression:\n";
    // str += "(fn " + uop->expr->to_string() + ")";
    return new HString(str);
  } else if (GenericFn *generic = dynamic_cast<GenericFn *>(arg_list.front())) {
    string str =
        "generic function:\n" + generic->docstring->value +
        "\nlambda-list: " + "\ntype: " + hydra_to_string(generic->type, r, s) +
        "\nfunctions: \n";

    for (Operator *op : generic->functions) {
      str += hydra_to_string(op, r, s) + "\n";
    }
    return new HString(str);
  }
  if (Operator *op = dynamic_cast<Operator *>(arg_list.front())) {
    string str = "function:\n" + op->docstring->value + "\nlambda-list: " +
                 "\ntype: " + hydra_to_string(op->type, r, s);
    return new HString(str);
  } else {
    return new HString("no documentation for this type");
  }
}

// currently returns a string of time, not a time object... fix this!!!
Object *op_time(list<Object *> arg_list, LocalRuntime &r, LexicalScope &s) {

  Object *obj = arg_list.front();
  steady_clock::time_point start = steady_clock::now();
  obj->eval(r, s);
  steady_clock::time_point end = steady_clock::now();

  duration<double> time = duration_cast<duration<double>>(end - start);

  return new HString(std::to_string(time.count()));
}

// currently returns a string of time, not a time object... fix this!!!
Object *op_macexpand(list<Object *> arg_list, LocalRuntime &r,
                     LexicalScope &s) {
  if (Cons *cns = dynamic_cast<Cons *>(arg_list.front())) {
    Object *oper = cns->car->eval(r, s);
    if (Operator *op = dynamic_cast<Operator *>(oper)) {
      if (!op->is_fn) {
        list<Object *> mac_list = op->get_arg_list(cns->cdr, r, s);
        Object *out = op->call(mac_list, r, s, true);
        return out;
        // return op_macexpand({out}, r, s);
      }
    }
  }
  return arg_list.front();
}

Operator *op::describe;
Operator *op::time;
Operator *op::macexpand;

void op::initialize_dev() {
  op::describe =
      new InbuiltOperator("doc", "describes the provided argument", op_describe,
                          type::Fn::with_args({new type::Any}), true);

  op::time = new InbuiltOperator(
      "time", "evaluates the argument(s), and returns a time object", op_time,
      type::Fn::with_args({new type::Any}), true);

  op::macexpand = new InbuiltOperator(
      "macroexpand", "Expands global macros within a program", op_macexpand,
      type::Fn::with_args({new type::Any}), true);
}
