#ifndef __HYDRA_EXPRESSIONS_HANDLER_HPP
#define __HYDRA_EXPRESSIONS_HANDLER_HPP

#include <list>

#include "expressions/object.hpp"
#include "expressions/symbol.hpp"
#include "expressions/operation.hpp"
#include "types/basic.hpp"

namespace expr {
struct condition_handler {
  virtual Object* handle(Object* condition) = 0;
};

struct bind_handler : public condition_handler {
  bind_handler(std::list<Object*>, interp::LocalRuntime& r, interp::LexicalScope &s);

  interp::LocalRuntime& r;
  interp::LexicalScope& s;
  std::list<Object*> handling_code;
  Object* handle(Object* condition);
};

struct case_handler : public condition_handler {
  Object* handle(Object* condition);
};


struct hydra_restart : public Operator {
  hydra_restart(Operator* o, Symbol* s);
  void mark_node();

  Object *call(std::list<Object*> arg_list, interp::LocalRuntime &r, interp::LexicalScope &s, bool macexpand = false);
  Operator* op;
  Symbol* sym;
};

enum exception_type {
  RESTART_CALL,
  CASE_THROW
};

struct hydra_exception {
  hydra_exception(exception_type t);

  // this used to be a union, but c++
  // didn't like it for some reason, so we
  // use tag:
  exception_type type;


  // FAKE UNION PT1
  // data for CASE_THROW
  Object *obj;

  // FAKE UNION PT2
  // data for RESTART_CALL
  hydra_restart *res;
  std::list<Object *> args;
  interp::LexicalScope *s;
};

}
#endif
