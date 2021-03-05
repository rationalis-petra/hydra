#include <list>
#include <time.h>
#include <chrono>

#include "expressions.hpp"
#include "operations.hpp"

using namespace std::chrono;
using std::list;
using std::string;

op_describe::op_describe() {
  is_fn = true;
  docstring = new hydra_string("describes the provided argument");
  type->arg_list.push_front(new type_nil);
};


hydra_object* op_describe::call(hydra_object* alist, runtime &r, lexical_scope& s) {
  list<hydra_object*> arg_list = get_arg_list(alist, r, s);

  if (user_oper* uop = dynamic_cast<user_oper*>(arg_list.front())) {
    string str = "generic function:\n" + uop->docstring->value +
                 "\nlambda-list: " +
      "\ntype: " + uop->type->to_string();
      //str += "\n expression:\n";
      //str += "(fn " + uop->expr->to_string() + ")";
    return new hydra_string(str);
  } else if (combined_fn* generic = dynamic_cast<combined_fn*>(arg_list.front())) {
    string str = "generic function:\n" + generic->docstring->value +
                 "\nlambda-list: " +
      "\ntype: " + generic->type->to_string();
    return new hydra_string(str);
  } if (hydra_oper* op = dynamic_cast<hydra_oper*>(arg_list.front())) {
    string str = "function:\n" + op->docstring->value +
                 "\nlambda-list: " +
      "\ntype: " + op->type->to_string();
    return new hydra_string(str);
  } else {
    return new hydra_string("no documentation for this type");
  }
}

// currently returns a string of time, not a time object... fix this!!!
op_time::op_time() {
  is_fn = true;
  docstring = new hydra_string("evaluates the argument(s), and returns a time object");
  type->arg_list.push_front(new type_nil);
};



hydra_object* op_time::call(hydra_object* alist, runtime &r, lexical_scope& s) {
  list<hydra_object*> arg_list = get_arg_list(alist, r, s);
  hydra_object* obj = arg_list.front();
  steady_clock::time_point start = steady_clock::now();
  obj->eval(r, s);
  steady_clock::time_point end = steady_clock::now();

  duration<double> time = duration_cast<duration<double>>(end - start);

  return new hydra_string(std::to_string(time.count()));
}
