#include <iostream>
#include <variant>

#include "expressions.hpp"
#include "operations.hpp"

using std::list;
using std::string;

using type::hydra_cast;

using namespace expr;

Value *op_if(Operator* op, Value *alist, LocalRuntime &r, LexicalScope &s) {
  list<Value *> arg_list = op->get_arg_list(alist, r, s);
  // aseert that list length is 3
  int len = arg_list.size();
  if (len != 3) {
    throw "arglist to if invalid size!";
  }
  // we now assume that arg_list is a list of length 3

  for (Value* v : arg_list)
    Value::roots.remove(v);

  Value *condition = arg_list.front()->eval(r, s);
  Value::roots.insert(condition);
  arg_list.pop_front();
  // is nil?
  if (condition->null()) {
    Value* out =  arg_list.back()->eval(r, s);
    Value::roots.remove(condition);
    return out;
  }
  // otherwise
  Value* out = arg_list.front()->eval(r, s);
  Value::roots.remove(condition);
  return out;
}

Operator *op::do_if =
  new InbuiltOperator("Takes three expressions: evaluates the first one.\n"
                      "If it is nil, then evaluate the second expression & return\n"
                      "Otherwise, evaluate the first expression.",
                      op_if,
                      type::Fn::with_args({new type::Any, new type::Any, new type::Any}),
                      false);

Value *op_while(Operator* op, Value *alist, LocalRuntime &r,
                             LexicalScope &s) {
  list<Value *> arg_list = op->get_arg_list(alist, r, s);
  // assert that list length is >2

  Value *condition = arg_list.front();
  arg_list.pop_front();
  // is nil?
  Value* out = nil::get();
  while (!condition->eval(r, s)->null()) {
    for (Value *o : arg_list) {
      out = o->eval(r, s);
    }
  }
  for (Value* v : arg_list) {
    Value::roots.remove(v);
  }
  // otherwise
  return out;
}

Operator* op::do_while =
  new InbuiltOperator("Evaluate the first expression. If it is nil, return\n"
                      "Otherwise, evaluate rest of the expression(s), and repeat",
                      op_while,
                      type::Fn::with_rest(new type::Any),
                      false);

Value *op_bind(Operator* op, Value *alist, LocalRuntime &r, LexicalScope &s) {
  list<Value *> arg_list = op->get_arg_list(alist, r, s);

  if (Symbol *symbol = dynamic_cast<Symbol *>(arg_list.front())) {
    Value *value = arg_list.back();

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


Operator* op::bind =
  new InbuiltOperator("Sets the value symbol (first argument) to the second argument",
                      op_bind,
                      type::Fn::with_args({new type::Symbol, new type::Any}),
                      true);

Value *op_unbind(Operator* op, Value *alist, LocalRuntime &r, LexicalScope &s) {
  list<Value *> arg_list = op->get_arg_list(alist, r, s);

  Symbol *symbol = dynamic_cast<Symbol *>(arg_list.front());

  // if there is a lexical scope with the value
  if (s.map.find(symbol) != s.map.end()) {
    //s.map[symbol] = nullptr;
    s.map.erase(symbol);
  } else if (symbol->mut) {
    symbol->value = nullptr;
  } else {
    string err = "Error: cannot unbind immutable symbol";
    throw err;
  }

  return symbol;
}

Operator* op::unbind = 
  new InbuiltOperator("Removes the value from a symbol",
                      op_unbind,
                      type::Fn::with_args({new type::Symbol}),
                      true);

Value* op_definedp(Operator* op, Value *alist, LocalRuntime &r, LexicalScope &s) {
  list<Value *> arg_list = op->get_arg_list(alist, r, s);
  Symbol* sym = hydra_cast<Symbol>(arg_list.front());
  if (sym->value) {
    return t::get();
  } else {
    return nil::get();
  }
}

Operator* op::definedp =
  new InbuiltOperator("Returns t if symbol contains a value, and nil otherwise",
                      op_definedp,
                      type::Fn::with_args({new type::Symbol}),
                      true);

Value *op_quote(Operator* op, Value *alist, LocalRuntime &r,
                             LexicalScope &s) {
  list<Value *> arg_list = op->get_arg_list(alist, r, s);
  return arg_list.front();
}


Operator* op::quote =
  new InbuiltOperator("Prevents evaluation of the argument it is provided",
                      op_quote,
                      type::Fn::with_args({new type::Any}),
                      false);

Value *op_eval(Operator* op, Value *alist, LocalRuntime &r, LexicalScope &s) {
  list<Value *> arg_list = op->get_arg_list(alist, r, s);
  if (arg_list.size() != 1) {
    string err = "invalid number of arguments to eval";
    throw err;
  }
  // eval evaluates in a null lexical scope!
  LexicalScope new_scope;
  Value* value = arg_list.front()->eval(r, new_scope);
  return value;
}


Operator* op::eval =
  new InbuiltOperator("Evaluates it's argument as if it was a hydra program",
                      op_eval,
                      type::Fn::with_args({new type::Any}),
                      true);

Value *op_progn(Operator* op, Value *alist, LocalRuntime &r,
                             LexicalScope &s) {
  list<Value *> arg_list = op->get_arg_list(alist, r, s);
  Value *out = nullptr;
  for (Value *arg : arg_list) {
    out = arg->eval(r, s);
  }
  if (!out)
    out = nil::get();
  return out;
}

Operator* op::progn =
  new InbuiltOperator("Evaluates its arguments in sequential order, returning the value\n"
                      "of the last one",
                      op_progn,
                      type::Fn::with_rest(new type::Any),
                      false);


Value *op_gen(Operator* op, Value *alist, LocalRuntime &r, LexicalScope &s) {
  list<Value*> arg_list = op->get_arg_list(alist, r, s);
  CombinedFn* out = new CombinedFn;
  out->type->rest_type = new type::Any;
  arg_list.pop_front();
  if (!arg_list.empty()) {
    if (HString* str = dynamic_cast<HString*>(arg_list.front())) {
      out->docstring = str;
    }
  }
  return out;
}

Operator* op::genfn =
  new InbuiltOperator("Generates a new generic function object",
                      op_gen,
                      type::Fn::with_all({new type::List}, new type::Any, new type::GenFn),
                      false);

Value *op_fn(Operator* op, Value *alist, LocalRuntime &r, LexicalScope &s) {
  return (Value*) new UserOperator(alist, true, r, s);
}

Operator* op::fn =
  new InbuiltOperator("Generates a new function object",
                      op_fn,
                      type::Fn::with_rest(new type::Any),
                      false);

Value *op_mac(Operator* op, Value *alist, LocalRuntime &r, LexicalScope &s) {
  return (Value*) new UserOperator(alist, false, r, s);
}


Operator* op::mac =
  new InbuiltOperator("Generates a new macro object",
                      op_mac,
                      type::Fn::with_all({new type::Cons}, new type::Any, new type::Mac),
                      false);

Value *op_addfn(Operator* op, Value *alist, LocalRuntime &r, LexicalScope &s) {
  list<Value*> arg_list = op->get_arg_list(alist, r, s);
  CombinedFn* f = dynamic_cast<CombinedFn*>(arg_list.front());
  arg_list.pop_front();

  for (Value* o : arg_list) {
    Operator* op = hydra_cast<Operator>(o);
    f->add(op);
  }

  return (Value*) f;
}

Operator* op::addfn =
  new InbuiltOperator("Combines functions into an effective function",
                      op_addfn,
                      type::Fn::with_all({new type::GenFn}, new type::Fn, new type::GenFn),
                      true);

Value *op_exit(Operator* op, Value *alist, LocalRuntime &r, LexicalScope &s) {
  for (Value* obj : Value::node_list) {
    delete obj;
  }
  exit(0);
}

Operator* op::exit =
  new InbuiltOperator("Exits the current running application",
                      op_exit,
                      type::Fn::with_args({}),
                      true);


Value *op_ref(Operator* op, Value *alist, LocalRuntime &r, LexicalScope &s) {
  list<Value*> arg_list = op->get_arg_list(alist, r, s);

  Ref* ref = new Ref;
  ref->ptr = arg_list.front();
  return ref;
}

Operator* op::ref  =
  new InbuiltOperator("Returns a reference to the provided value",
                      op_ref,
                      type::Fn::with_args({new type::Any}),
                      true);

// op_var::op_var() {
//   is_fn = true;
//   docstring = new HString("Creates some mutable storage, and returns a handle to it");
//   type->arg_list.push_front(new type_nil);
// }

// Value *op_var::call(Value *alist, LocalRuntime &r, LexicalScope &s) {
//   list<Value*> arg_list = get_arg_list(alist, r, s);

//   hydra_var* var = new hydra_var;
//   // TODO: add get-var function to objects?
//   var->val = arg_list.front();
//   return var;
// }
