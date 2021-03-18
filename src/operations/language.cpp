#include <iostream>
#include <variant>

#include "expressions.hpp"
#include "operations.hpp"

using std::list;
using std::string;

using type::hydra_cast;

using namespace expr;

Object *op_if(list<Object *> arg_list, LocalRuntime &r, LexicalScope &s) {

  for (Object *v : arg_list)
    Object::roots.remove(v);

  Object *condition = arg_list.front()->eval(r, s);
  Object::roots.insert(condition);
  arg_list.pop_front();
  // is nil?
  if (condition->null()) {
    Object *out = arg_list.back()->eval(r, s);
    Object::roots.remove(condition);
    return out;
  }
  // otherwise
  Object *out = arg_list.front()->eval(r, s);
  Object::roots.remove(condition);
  return out;
}

Object *op_while(list<Object *> arg_list, LocalRuntime &r, LexicalScope &s) {

  // assert that list length is >2

  Object *condition = arg_list.front();
  arg_list.pop_front();
  // is nil?
  Object *out = nil::get();
  while (!condition->eval(r, s)->null()) {
    for (Object *o : arg_list) {
      out = o->eval(r, s);
    }
  }
  for (Object *v : arg_list) {
    Object::roots.remove(v);
  }
  // otherwise
  return out;
}

Object *op_bind(list<Object *> arg_list, LocalRuntime &r, LexicalScope &s) {

  if (Symbol *symbol = dynamic_cast<Symbol *>(arg_list.front())) {
    Object *value = arg_list.back();

    // if there is a lexical scope with the value
    if (s.map.find(symbol) != s.map.end()) {
      s.map[symbol] = value;
    } else if (symbol->mut) {
      symbol->value = value;
    } else {
      string err = "Error: cannot bind immutable symbol";
      throw err;
    }

    return value;
  } else {
    string err = "Error: provided non-symbol as first argument of set";
    throw err;
  }
}

Object *op_unbind(list<Object *> arg_list, LocalRuntime &r, LexicalScope &s) {

  Symbol *symbol = dynamic_cast<Symbol *>(arg_list.front());

  // if there is a lexical scope with the value
  if (s.map.find(symbol) != s.map.end()) {
    // s.map[symbol] = nullptr;
    s.map.erase(symbol);
  } else if (symbol->mut) {
    symbol->value = nullptr;
  } else {
    string err = "Error: cannot unbind immutable symbol";
    throw err;
  }

  return symbol;
}

Object *op_definedp(list<Object *> arg_list, LocalRuntime &r, LexicalScope &s) {

  Symbol *sym = hydra_cast<Symbol>(arg_list.front());
  auto loc = s.map.find(sym);
  if (loc != s.map.end()) {
    return t::get();
  }

  if (sym->value) {
    return t::get();
  } else {
    return nil::get();
  }
}

Object *op_quote(list<Object *> arg_list, LocalRuntime &r, LexicalScope &s) {

  return arg_list.front();
}

Object *op_eval(list<Object *> arg_list, LocalRuntime &r, LexicalScope &s) {

  if (arg_list.size() != 1) {
    string err = "invalid number of arguments to eval";
    throw err;
  }
  // eval evaluates in a null lexical scope!
  LexicalScope new_scope;
  Object *value = arg_list.front()->eval(r, new_scope);
  return value;
}

Object *op_progn(list<Object *> arg_list, LocalRuntime &r, LexicalScope &s) {

  Object *out = nullptr;
  for (Object *arg : arg_list) {
    out = arg->eval(r, s);
  }
  if (!out)
    out = nil::get();
  return out;
}

Object *op_exit(list<Object *> arg_list, LocalRuntime &r, LexicalScope &s) {
  for (Object *obj : Object::node_list) {
    delete obj;
  }
  exit(0);
}

Object *op_set_invoker(list<Object *> arg_list, LocalRuntime &r,
                       LexicalScope &s) {
  if (Operator *op = dynamic_cast<Operator *>(arg_list.back())) {
    arg_list.front()->invoker = op;
    return op;
  } else {
    // type
    string err = "type error in op-set-invoker";
    throw err;
  }
}

Object *op_get_invoker(list<Object *> arg_list, LocalRuntime &r,
                       LexicalScope &s) {
  if (Operator *op = dynamic_cast<Operator *>(arg_list.back())) {
    arg_list.front()->invoker = op;
    return op;
  } else {
    // type
    string err = "type error in op-set-invoker";
    throw err;
  }
}

Operator *op::do_if;
Operator *op::do_while;
Operator *op::bind;
Operator *op::unbind;
Operator *op::definedp;
Operator *op::quote;
Operator *op::eval;
Operator *op::progn;
Operator *op::exit;
Operator *op::set_invoker;
Operator *op::get_invoker;

void op::initialize_language() {
  op::do_if = new InbuiltOperator(
      "Takes three expressions: evaluates the first one.\n"
      "If it is nil, then evaluate the second expression & return\n"
      "Otherwise, evaluate the first expression.",
      op_if, type::Fn::with_args({new type::Any, new type::Any, new type::Any}),
      false);
  op::do_while = new InbuiltOperator(
      "Evaluate the first expression. If it is nil, return\n"
      "Otherwise, evaluate rest of the expression(s), and repeat",
      op_while, type::Fn::with_rest(new type::Any), false);
  op::bind = new InbuiltOperator(
      "Sets the value symbol (first argument) to the second argument", op_bind,
      type::Fn::with_args({new type::Symbol, new type::Any}), true);

  op::unbind =
      new InbuiltOperator("Removes the value from a symbol", op_unbind,
                          type::Fn::with_args({new type::Symbol}), true);
  op::definedp = new InbuiltOperator(
      "Returns t if symbol contains a value, and nil otherwise", op_definedp,
      type::Fn::with_args({new type::Symbol}), true);
  op::quote = new InbuiltOperator(
      "Prevents evaluation of the argument it is provided", op_quote,
      type::Fn::with_args({new type::Any}), false);
  op::eval = new InbuiltOperator(
      "Evaluates it's argument as if it was a hydra program", op_eval,
      type::Fn::with_args({new type::Any}), true);
  op::progn = new InbuiltOperator(
      "Evaluates its arguments in sequential order, returning the value\n"
      "of the last one",
      op_progn, type::Fn::with_rest(new type::Any), false);
  op::exit = new InbuiltOperator("Exits the current running application",
                                 op_exit, type::Fn::with_args({}), true);
  op::set_invoker = new InbuiltOperator(
      "Sets the invoker: an object which is used when this object\n"
      "is called, allowing it to act as a function.",
      op_set_invoker, type::Fn::with_args({new type::Any, new type::Fn}), true);
  op::get_invoker = new InbuiltOperator(
      "Retrieves the invoker for this obejct.", op_get_invoker,
      type::Fn::with_all({new type::Any}, nullptr, new type::Fn), true);
}
