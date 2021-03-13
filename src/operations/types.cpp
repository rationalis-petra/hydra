#include <list>
#include <string>

#include "expressions.hpp"
#include "operations/types.hpp"
#include "types.hpp"

using std::list;
using std::string;

using namespace expr;

Object *op_typep(Operator *op, Object *alist, LocalRuntime &r, LexicalScope &s) {
  list<Object *> arg_list = op->get_arg_list(alist, r, s);
  Object *obj = arg_list.back();
  type::Type *type_rep = dynamic_cast<type::Type *>(arg_list.front());

  return type_rep->check_type(obj);
}

Operator *op::typep = new InbuiltOperator(
    "Returns t if the first argument is the type defined by the\n"
    "second, otherwise returns nil",
    op_typep, type::Fn::with_args({new type::MetaType, new type::Any}), true);

Object *op_mk_type(Operator *op, Object *alist, LocalRuntime &r,
                  LexicalScope &s) {
  list<Object *> arg_list = op->get_arg_list(alist, r, s);
  type::TypeConstructor *type =
      dynamic_cast<type::TypeConstructor *>(arg_list.front());
  arg_list.pop_front();

  return type->constructor(arg_list);
}

Operator *op::mk_type = new InbuiltOperator(
    "Calls the constructor for a particular type", op_mk_type,
    type::Fn::with_all({new type::MetaConstructor}, new type::Any,
                       new type::MetaType),
    true);

Object *op_subtype(Operator *op, Object *alist, LocalRuntime &r,
                  LexicalScope &s) {
  list<Object *> arg_list = op->get_arg_list(alist, r, s);

  // TODO: implement
  return t::get();
}

Operator *op::subtype = new InbuiltOperator(
    "Returns true if the first argument is a subtype of the second", op_subtype,
    type::Fn::with_args({new type::MetaType, new type::MetaType}), true);
