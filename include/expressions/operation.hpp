#ifndef __HYDRA_EXPRESSIONS_OPERATIONS_HPP
#define __HYDRA_EXPRESSIONS_OPERATIONS_HPP

#include <map>
#include <set>
#include <string>
#include <functional>

#include "object.hpp"

namespace type {
  struct Fn;
}

namespace expr {

struct Symbol;
struct Vector;
struct HString;

struct Operator : public Value {
  // FUNCTIONS
  Operator();

  virtual void mark_node();
  std::string to_string() const;
  bool is_fn;

  virtual Value *call(Value *arg_list, LocalRuntime &r,
                             LexicalScope &s) = 0;
  std::list<Value *> get_arg_list(Value *arg_list, LocalRuntime &r,
                                         LexicalScope &s);
  // STATE
  type::Fn* type;
  HString *docstring;
};

struct UserOperator : public Operator {
  virtual void mark_node();

  ~UserOperator();
  std::string to_string() const;
  Value *call(Value *arg_list, LocalRuntime &r, LexicalScope &s);

  Symbol *rest; // for accepting variable arguments
  Symbol *self; // name of function within function's scope
  std::list<Symbol *> optionals;
  std::map<Symbol *, Symbol*> keys;

  std::list<Symbol *> arg_names;
  Value *expr;

  // captured scope for closures
  LexicalScope *scope;
  UserOperator(Value *op_def, bool eval_args, LocalRuntime &r, LexicalScope &s);
};

struct CombinedFn : public Operator {
  // FUNTCIONS
  virtual void mark_node();

  void add (Operator* op);
  std::string to_string() const;
  Value *call(Value *arg_list, LocalRuntime &r, LexicalScope &s);

  //STATE
  std::list<Operator*> functions;
};

struct InbuiltOperator : public Operator {
  // FUNCTIONS
  InbuiltOperator(std::string str,
                  Value* (*call)(Operator *op, Value *arg_list, LocalRuntime &r,
                              LexicalScope &s),
                  type::Fn *t, bool is_fn);

  Value *call(Value *arg_list, LocalRuntime &r, LexicalScope &s);

  // STATE
  Value *(*fnc)(Operator *op, Value *arg_list, LocalRuntime &r, LexicalScope &s);
};

} // namespace expr

#endif
