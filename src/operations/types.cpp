#include <list>
#include <string>

#include "expressions.hpp"
#include "operations/types.hpp"
#include "types.hpp"

using std::list;
using std::string;

using namespace expr;

Object *op_typep(list<Object*> arg_list, LocalRuntime &r, LexicalScope &s) {
  
  Object *obj = arg_list.back();
  type::Type *type_rep = dynamic_cast<type::Type *>(arg_list.front());

  return type_rep->check_type(obj);
}

Operator *op::typep = new InbuiltOperator(
    "Returns t if the first argument is the type defined by the\n"
    "second, otherwise returns nil",
    op_typep, type::Fn::with_args({new type::MetaType, new type::Any}), true);

Object *op_mk_type(list<Object*> arg_list, LocalRuntime &r,
                  LexicalScope &s) {
  
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

Object *op_subtype(list<Object*> arg_list, LocalRuntime &r,
                  LexicalScope &s) {
  
  type::Type* t1 = dynamic_cast<type::Type*>(arg_list.front());
  type::Type* t2 = dynamic_cast<type::Type*>(arg_list.back());

  return t1->subtype(t2);
}

Operator *op::subtype = new InbuiltOperator(
    "Returns true if the first argument is a subtype of the second", op_subtype,
    type::Fn::with_args({new type::MetaType, new type::MetaType}), true);
