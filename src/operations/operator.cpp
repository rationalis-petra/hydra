#include <iostream>
#include <variant>

#include "expressions.hpp"
#include "operations.hpp"

using std::list;
using std::string;

using type::hydra_cast;

using namespace expr;

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
  if (!arg_list.empty()) {
    if (HString *str = dynamic_cast<HString *>(arg_list.front())) {
      out->docstring = str;
    }
  }
  return out;
}

Object *op_addfn(list<Object *> arg_list, LocalRuntime &r, LexicalScope &s) {
  GenericFn *f = dynamic_cast<GenericFn *>(arg_list.front());
  arg_list.pop_front();

  for (Object *o : arg_list) {
    Operator *op = hydra_cast<Operator>(o);
    f->add_safe(op, r, s);
  }

  return (Object *)f;
}

Operator *op::fn;
Operator *op::mac;
Operator *op::genfn;
Operator *op::addfn;

void op::initialize_mkoperator() {

  op::fn = new InbuiltOperator("Generates a new function object", op_fn,
                               type::Fn::with_rest(new type::Any), false);

  op::mac = new InbuiltOperator(
      "Generates a new macro object", op_mac,
      type::Fn::with_all({new type::Cons}, new type::Any, new type::Mac),
      false);

  op::genfn = new InbuiltOperator(
      "Generates a new generic function object", op_gen,
      type::Fn::with_all({new type::List}, new type::Any, new type::GenFn),
      false);

  op::addfn = new InbuiltOperator(
      "Combines functions into an effective function", op_addfn,
      type::Fn::with_all({new type::GenFn}, new type::Fn, new type::GenFn),
      true);
}
