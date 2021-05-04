#include <iostream>
#include <variant>

#include "expressions.hpp"
#include "operations.hpp"
#include "utils.hpp"

using std::list;
using std::string;

using type::hydra_cast;

using namespace expr;
using namespace interp;

Object *op_if(list<Object *> arg_list, LocalRuntime &r, LexicalScope &s) {

  for (Object *v : arg_list)
    Object::collector->remove_root(v);

  Object *condition = arg_list.front()->eval(r, s);
  Object::collector->insert_root(condition);
  arg_list.pop_front();
  // is nil?
  if (condition->null()) {
    Object *out = arg_list.back()->eval(r, s);
    Object::collector->remove_root(condition);
    return out;
  }
  // otherwise
  Object *out = arg_list.front()->eval(r, s);
  Object::collector->remove_root(condition);
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
    Object::collector->remove_root(v);
  }
  // otherwise
  return out;
}

Object *op_bind(list<Object *> arg_list, LocalRuntime &r, LexicalScope &s) {

  if (Symbol *symbol = get_inbuilt<Symbol *>(arg_list.front())) {
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

Object *op_boundp(list<Object *> arg_list, LocalRuntime &r, LexicalScope &s) {

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
  //Object::collector->exit_cleanup();
  exit(0);
}


Operator *op::do_if;
Operator *op::do_while;
Operator *op::bind;
Operator *op::unbind;
Operator *op::boundp;
Operator *op::quote;
Operator *op::eval;
Operator *op::progn;
Operator *op::exit;

void op::initialize_language() {
  op::do_if = new InbuiltOperator(
      "if",
      "Takes three expressions: evaluates the first one.\n"
      "If it is nil, then evaluate the second expression & return\n"
      "Otherwise, evaluate the first expression.",
      op_if, type::Fn::with_args({new type::Any, new type::Any, new type::Any}),
      false);
  op::do_while = new InbuiltOperator(
      "while",
      "Evaluate the first expression. If it is nil, return\n"
      "Otherwise, evaluate rest of the expression(s), and repeat",
      op_while, type::Fn::with_rest(new type::Any), false);
  op::bind = new InbuiltOperator(
      "bind", "Sets the value symbol (first argument) to the second argument",
      op_bind, type::Fn::with_args({type::symbol_type, new type::Any}), true);

  op::unbind = new InbuiltOperator(
      "unbind", "Removes the value from a symbol", op_unbind,
      type::Fn::with_args({type::symbol_type}), true);
  op::boundp = new InbuiltOperator(
      "defined?", "Returns t if symbol contains a value, and nil otherwise",
      op_boundp, type::Fn::with_args({type::symbol_type}), true);
  op::quote = new InbuiltOperator(
      "quote", "Prevents evaluation of the argument it is provided", op_quote,
      type::Fn::with_args({new type::Any}), false);
  op::eval = new InbuiltOperator(
      "eval", "Evaluates it's argument as if it was a hydra program", op_eval,
      type::Fn::with_args({new type::Any}), true);
  op::progn = new InbuiltOperator(
      "progn",
      "Evaluates its arguments in sequential order, returning the value\n"
      "of the last one",
      op_progn, type::Fn::with_rest(new type::Any), false);
  op::exit =
      new InbuiltOperator("exit", "Exits the current running application",
                          op_exit, type::Fn::with_args({}), true);
}
