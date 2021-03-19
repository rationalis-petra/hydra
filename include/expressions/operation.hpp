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

struct Operator : public Object {
  // FUNCTIONS
  Operator();

  virtual void mark_node();
  std::string to_string(LocalRuntime &r, LexicalScope& s);
  bool is_fn;

  Object* operator()(std::list<Object*> arg_list, LocalRuntime &r, LexicalScope&s, bool macexpand = false);
  virtual Object *call(std::list<Object*> arg_list, LocalRuntime &r, LexicalScope &s, bool macexpand = false) = 0;
  std::list<Object *> get_arg_list(Object* arg_list, LocalRuntime &r, LexicalScope &s);

  // STATE
  type::Fn* type;
  HString *docstring;
};

struct UserOperator : public Operator {
  virtual void mark_node();

  ~UserOperator();
  std::string to_string(LocalRuntime &r, LexicalScope &s);
  Object *call(std::list<Object*> arg_list, LocalRuntime &r, LexicalScope &s, bool macexpand = false);

  Symbol *rest; // for accepting variable arguments
  Symbol *self; // name of function within function's scope
  std::list<Symbol *> optionals;
  std::map<Symbol *, Symbol*> keys;

  std::list<Symbol *> arg_names;
  Object *expr;

  // captured scope for closures
  LexicalScope *scope;
  UserOperator(std::list<Object*> op_def, bool eval_args, LocalRuntime &r, LexicalScope &s);
};

struct GenericFn : public Operator {
  // FUNTCIONS
  GenericFn();
  virtual void mark_node();

  void add (Operator* op);
  void add_safe (Operator* op, LocalRuntime& r, LexicalScope& s);
  std::string to_string(LocalRuntime &r, LexicalScope &s);
  Object *call(std::list<Object*> arg_list, LocalRuntime &r, LexicalScope &s, bool macexpand = false);

  //STATE
  std::list<Operator*> functions;
};

struct InbuiltOperator : public Operator {
  // FUNCTIONS
  InbuiltOperator(std::string str,
                  Object* (*call)(std::list<Object*> arg_list, LocalRuntime &r,
                              LexicalScope &s),
                  type::Fn *t, bool is_fn);

  Object *call(std::list<Object*> arg_list, LocalRuntime &r, LexicalScope &s, bool macexpand = false);

  // STATE
  Object *(*fnc)(std::list<Object*>arg_list, LocalRuntime &r, LexicalScope &s);
};


struct NextFnc : public Operator {
  // FUNCTIONS
  NextFnc(std::list<Operator*> funcs, std::list<Object*> arg_list, Symbol* nextsym);
  Object *call(std::list<Object*> arg_list, LocalRuntime&r, LexicalScope& s, bool macexpand = false);

  Symbol* nextsym;
  std::list<Operator*> funcs;
  std::list<Object*> arg_list;
};

} // namespace expr

#endif
