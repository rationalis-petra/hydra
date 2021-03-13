#ifndef __HYDRA_EXPRESSIONS_HANDLER_HPP
#define __HYDRA_EXPRESSIONS_HANDLER_HPP

#include "expressions/object.hpp"
#include "expressions/language.hpp"
#include "expressions/operation.hpp"
#include "types/basic.hpp"

namespace expr {
struct condition_handler {
  virtual Object* handle(Object* condition) = 0;
};

struct bind_handler : public condition_handler {
  bind_handler(std::list<Object*>, LocalRuntime& r, LexicalScope &s);

  LocalRuntime& r;
  LexicalScope& s;
  std::list<Object*> handling_code;
  Object* handle(Object* condition);
};

struct case_handler : public condition_handler {
  Object* handle(Object* condition);
};

enum exception_type {
  RESTART_CALL,
  CASE_THROW
};


struct hydra_restart : public Operator {
  hydra_restart(Operator* o, Symbol* s);
  void mark_node();

  Object *call(Object *arg_list, LocalRuntime &r, LexicalScope &s);
  Operator* op;
  Symbol* sym;
};


struct hydra_exception {
  hydra_exception(exception_type t);

  exception_type type;

  union {
    struct {
      Object* obj;
    };
    struct {
      hydra_restart *res;
      Object *args;
      LexicalScope *s;
    };
  };
};

}
#endif
