#ifndef __HYDRA_EXPRESSIONS_OPERATIONS_HPP
#define __HYDRA_EXPRESSIONS_OPERATIONS_HPP

#include <map>
#include <set>
#include <string>

#include "object.hpp"

struct hydra_symbol;
struct type_fn;
struct hydra_string;

struct hydra_oper : public hydra_object {
  virtual void mark_node();
  hydra_oper();
  std::string to_string() const;
  bool is_fn;

  type_fn* type;
  hydra_string *docstring;

  virtual hydra_object *call(hydra_object *arg_list, runtime &r,
                             lexical_scope &s) = 0;

protected:
  std::list<hydra_object *> get_arg_list(hydra_object *arg_list, runtime &r,
                                         lexical_scope &s);
};

struct user_oper : public hydra_oper {
  virtual void mark_node();

  ~user_oper();
  std::string to_string() const;
  hydra_object *call(hydra_object *arg_list, runtime &r, lexical_scope &s);

  hydra_symbol *rest; // for accepting variable arguments
  hydra_symbol *self; // name of function within function's scope
  std::list<hydra_symbol *> optionals;
  std::map<hydra_symbol *, hydra_symbol*> keys;

  std::list<hydra_symbol *> arg_names;
  hydra_object *expr;

  // captured scope for closures
  lexical_scope *scope;
  user_oper(hydra_object *op_def, bool eval_args, runtime &r, lexical_scope &s);
};

struct combined_fn : public hydra_oper {
  virtual void mark_node();

  void add (hydra_oper* op);
  std::string to_string() const;
  hydra_object *call(hydra_object *arg_list, runtime &r, lexical_scope &s);

  std::list<hydra_oper*> functions;
};

#endif
