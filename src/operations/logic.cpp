#include <list>

#include "expressions.hpp"
#include "operations.hpp"

using std::list;
using std::string;

using namespace expr;


Value *op_eq(Operator* op, Value* alist, LocalRuntime& r, LexicalScope &s) {
  list<Value*> arg_list = op->get_arg_list(alist, r, s);
  if (arg_list.size() < 2) {
    string err = "Invalid number of arguments to macro =: expects 2";
    throw err;
  }
  Value* arg1 = arg_list.front();
  arg_list.pop_front();
  Value* arg2 = arg_list.front();
  if (Integer* num1 = dynamic_cast<Integer*>(arg1)) {
    if (Integer* num2 = dynamic_cast<Integer*>(arg2)) {
      if (num1->value == num2->value)
        return t::get();
      return nil::get();
    }
    return nil::get();
  }
  if (Char* char1 = dynamic_cast<Char*>(arg1)) {
    if (Char* char2 = dynamic_cast<Char*>(arg2)) {
      if (char1->value == char2->value)
        return t::get();
      return nil::get();
    }
    return nil::get();
  }
  if (Symbol* sym1 = dynamic_cast<Symbol*>(arg1)) {
    if (Symbol* sym2 = dynamic_cast<Symbol*>(arg2)) {
      if (sym1 == sym2)
        return t::get();
      return nil::get();
    }
    return nil::get();
  }
  if (HString* str1 = dynamic_cast<HString*>(arg1)) {
    if (HString* str2 = dynamic_cast<HString*>(arg2)) {
      if (str1->value == str2->value) 
        return t::get();
      return nil::get();
    }
  }
  if (dynamic_cast<nil*>(arg1)) {
    if (dynamic_cast<nil*>(arg2))
      return t::get();
    return nil::get();
  }
  return nil::get();
}

Operator* op::eq =
  new InbuiltOperator("Returns t if two values are equal, and nil otherwise",
                      op_eq,
                      type::Fn::with_args({new type::Nil, new type::Nil}),
                      true);

Value *op_or(Operator* op, Value* alist, LocalRuntime& r, LexicalScope &s) {
  list<Value*> arg_list = op->get_arg_list(alist, r, s);
  if (arg_list.size() < 2) {
    string err = "Invalid number of arguments to macro or";
    throw err;
  }

  for (Value* arg : arg_list) {
    Value* cond = arg->eval(r, s);
    if (!cond->null())
      return cond;
  }
  return nil::get();
}

Operator* op::do_or =
  new InbuiltOperator("Returns the first non-nil argument, or nil if all arguments are nil",
                      op_or,
                      type::Fn::with_rest(new type::Nil),
                      false);

Value *op_and(Operator* op, Value* alist, LocalRuntime& r, LexicalScope &s) {
  list<Value*> arg_list = op->get_arg_list(alist, r, s);
  if (arg_list.size() < 2) {
    string err = "Invalid number of arguments to macro and";
    throw err;
  }

  Value* out = nullptr;
  for (Value* arg : arg_list) {
    Value* cond = arg->eval(r, s);
    if (cond->null()) {
      return nil::get();
    }
    else {
      out = cond;
    }
  }
  if (!out)
    out = nil::get();
  return out;
}

Operator* op::do_and = 
  new InbuiltOperator("Returns the first nil argument, or the last argument if all are non-nil",
                      op_and,
                      type::Fn::with_rest(new type::Nil),
                      false);


Value *op_not(Operator* op, Value* alist, LocalRuntime& r, LexicalScope &s) {
  list<Value*> arg_list = op->get_arg_list(alist, r, s);
  if (arg_list.size() != 1) {
    string err = "Invalid number of arguments to macro not";
    throw err;
  }
  if (arg_list.front()->null()) {
    return t::get();
  } else {
    return nil::get();
  }
}

Operator* op::do_not =
  new InbuiltOperator("Returns t if argument is nil, and nil otherwise",
                      op_not,
                      type::Fn::with_args({new type::Nil}),
                      true);
