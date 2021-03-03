#ifndef __HYDRA_EXPRESSIONS_HANDLER_HPP
#define __HYDRA_EXPRESSIONS_HANDLER_HPP

#include "expressions/object.hpp"
#include "expressions/language.hpp"
#include "expressions/operation.hpp"
#include "types/basic.hpp"

struct condition_handler {
  virtual hydra_object* handle(hydra_object* condition) = 0;
};

struct bind_handler : public condition_handler {
  runtime& r;
  lexical_scope& s;
  std::list<hydra_object*> handling_code;
  bind_handler(std::list<hydra_object*>, runtime& r, lexical_scope &s);
  hydra_object* handle(hydra_object* condition);
};

struct case_handler : public condition_handler {
  hydra_object* handle(hydra_object* condition);
};

enum exception_type {
  RESTART_CALL,
  CASE_THROW
};


struct hydra_restart : public hydra_oper {
  hydra_restart(hydra_oper* o, hydra_symbol* s);
  void mark_node();

  hydra_object *call(hydra_object *arg_list, runtime &r, lexical_scope &s);
  hydra_oper* op;
  hydra_symbol* sym;
};


struct hydra_exception {
  hydra_exception(exception_type t);

  exception_type type;

  union {
    struct {
      hydra_object* obj;
    };
    struct {
      hydra_restart *res;
      hydra_object *args;
      lexical_scope *s;
    };
  };
};

#endif
