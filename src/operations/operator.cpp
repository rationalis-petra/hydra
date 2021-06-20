#include <iostream>
#include <variant>

#include "expressions.hpp"
#include "operations.hpp"
#include "utils.hpp"

using std::list;
using std::string;

using namespace expr;
using namespace interp;

Object *op_fn(list<Object *> arg_list, LocalRuntime &r, LexicalScope &s) {
  return (Object *)new UserOperator(arg_list, true, r, s);
}

Object *op_mac(list<Object *> arg_list, LocalRuntime &r, LexicalScope &s) {
  return (Object *)new UserOperator(arg_list, false, r, s);
}

Object *op_gen(list<Object *> arg_list, LocalRuntime &r, LexicalScope &s) {

  GenericFn *out = new GenericFn;
  out->type->rest_type = new type::Any;
  arg_list.pop_front();
  // TODO: flesh me out!
  return out;
}

Object *op_addfn(list<Object *> arg_list, LocalRuntime &r, LexicalScope &s) {
  GenericFn *f = dynamic_cast<GenericFn *>(arg_list.front());
  arg_list.pop_front();

  for (Object *o : arg_list) {
    Operator *op = get_inbuilt<Operator*>(o);
    f->add_safe(op, r, s);
  }

  return (Object *)f;
}

Operator *op::fn;
Operator *op::mac;
Operator *op::genfn;
Operator *op::addfn;

void op::initialize_mkoperator() {

  op::fn = new InbuiltOperator("fn", "Generates a new function object", op_fn,
                               type::Fn::with_rest(new type::Any), false);

  op::mac = new InbuiltOperator(
      "mac", "Generates a new macro object", op_mac,
      type::Fn::with_all({new type::Cons}, new type::Any, new type::Mac),
      false);

  op::genfn = new InbuiltOperator(
      "gen", "Generates a new generic function object", op_gen,
      type::Fn::with_all({new type::List}, new type::Any, new type::GenFn),
      false);

  op::addfn = new InbuiltOperator(
      "addfn", "Combines functions into an effective function", op_addfn,
      type::Fn::with_all({new type::GenFn}, new type::Fn, new type::GenFn),
      true);
}
