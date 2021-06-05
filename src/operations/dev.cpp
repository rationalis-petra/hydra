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

Object *op_describe_any(list<Object *> arg_list, LocalRuntime &r, LexicalScope &s) {
  Object* obj = arg_list.front();
  Symbol* dsym = get_keyword("");
  auto it = obj->metadata.find(dsym);
  if (it != obj->metadata.end()) {
    return new HString(hydra_to_string(it->second, r, s));
  } else {
    return new HString("No docstring found for this object");
  }
}

Object *op_describe_generic(list<Object *> arg_list, LocalRuntime &r,
                            LexicalScope &s) {

  GenericFn *generic = get_inbuilt<GenericFn *>(arg_list.front());
  Symbol *dstring = get_keyword("docstring");
  string str =
      "generic function:\n" +
      hydra_to_string(generic->metadata[dstring], r, s) +
      "\nlambda-list: " + "\ntype: " + hydra_to_string(generic->type, r, s) +
      "\nfunctions: \n";

  for (Operator *op : generic->functions) {
    str += hydra_to_string(op, r, s) + "\n";
  }
  return new HString(str);
}


Object *op_describe_operator(list<Object *> arg_list, LocalRuntime &r,
                             LexicalScope &s) {
  Operator *op = get_inbuilt<Operator *>(arg_list.front());
  Symbol *dstring = get_keyword("docstring");
  string str = "function:\n" + hydra_to_string(op->metadata[dstring], r, s) +
               "\nlambda-list: " + "\ntype: " + hydra_to_string(op->type, r, s);
  return new HString(str);
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

GenericFn *op::describe;
GenericFn *op::time;
GenericFn *op::macexpand;

void op::initialize_dev() {

  op::describe = new GenericFn("doc");
  op::describe->type = type::Fn::with_args({new type::Any});
  op::time = new GenericFn("time");
  op::time->type = type::Fn::with_args({new type::Any});
  op::macexpand = new GenericFn("macro-expand");
  op::macexpand->type = type::Fn::with_args({new type::Any});

  op::describe->add(new InbuiltOperator(
      "doc", "gets the docstring for the argument", op_describe_any,
      type::Fn::with_args({new type::Any}), true));

  op::describe->add(new InbuiltOperator(
      "doc", "describes the provided argument", op_describe_generic,
      type::Fn::with_args({new type::GenFn}), true));

  op::describe->add(new InbuiltOperator(
      "doc", "describes the provided argument", op_describe_operator,
      type::Fn::with_args({new type::Fn}), true));

  op::time->add(new InbuiltOperator(
      "time", "evaluates the argument(s), and returns a time object", op_time,
      type::Fn::with_args({new type::Any}), true));

  op::macexpand->add(new InbuiltOperator(
      "macroexpand", "Expands global macros within a program", op_macexpand,
      type::Fn::with_args({new type::Any}), true));
}
