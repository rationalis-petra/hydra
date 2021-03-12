#include <list>
#include <time.h>
#include <chrono>

#include "expressions.hpp"
#include "operations.hpp"

using namespace std::chrono;
using namespace expr;

using std::list;
using std::string;


Value* op_describe(Operator* op, Value* alist, LocalRuntime &r, LexicalScope& s) {
  list<Value*> arg_list = op->get_arg_list(alist, r, s);

  if (UserOperator* uop = dynamic_cast<UserOperator*>(arg_list.front())) {
    string str = "generic function:\n" + uop->docstring->value +
                 "\nlambda-list: " +
      "\ntype: " + uop->type->to_string();
      //str += "\n expression:\n";
      //str += "(fn " + uop->expr->to_string() + ")";
    return new HString(str);
  } else if (CombinedFn* generic = dynamic_cast<CombinedFn*>(arg_list.front())) {
    string str = "generic function:\n" + generic->docstring->value +
                 "\nlambda-list: " +
      "\ntype: " + generic->type->to_string();
    return new HString(str);
  } if (Operator* op = dynamic_cast<Operator*>(arg_list.front())) {
    string str = "function:\n" + op->docstring->value +
                 "\nlambda-list: " +
      "\ntype: " + op->type->to_string();
    return new HString(str);
  } else {
    return new HString("no documentation for this type");
  }
}

Operator* op::describe =
  new InbuiltOperator("describes the provided argument",
                      op_describe,
                      type::Fn::with_args({new type::Nil}),
                      true);

// currently returns a string of time, not a time object... fix this!!!
Value* op_time(Operator* op, Value* alist, LocalRuntime &r, LexicalScope& s) {
  list<Value*> arg_list = op->get_arg_list(alist, r, s);
  Value* obj = arg_list.front();
  steady_clock::time_point start = steady_clock::now();
  obj->eval(r, s);
  steady_clock::time_point end = steady_clock::now();

  duration<double> time = duration_cast<duration<double>>(end - start);

  return new HString(std::to_string(time.count()));
}

Operator* op::time =
  new InbuiltOperator("evaluates the argument(s), and returns a time object",
                      op_time,
                      type::Fn::with_args({new type::Nil}),
                      true);
