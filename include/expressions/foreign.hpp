#ifndef __HYDRA_EXPRESSIONS_FOREIGN_HPP
#define __HYDRA_EXPRESSIONS_FOREIGN_HPP

#include <list>
#include <string>

#include "ffi.h"
#include "ltdl.h"

#include "object.hpp"
#include "types.hpp"

namespace expr {

struct CFnType;

struct ForeignLib : public Object {
  virtual void mark_node();

  ForeignLib(lt_dlhandle lib);
  std::string to_string(interp::LocalRuntime &r, interp::LexicalScope &s);

  lt_dlhandle lib;
};

struct ForeignSymbol : public Object {
  virtual void mark_node();

  ForeignSymbol(void *addr);
  std::string to_string(interp::LocalRuntime &r, interp::LexicalScope &s);

  void *address;
};

struct ForeignOperator : public Operator {
  ForeignOperator(CFnType* type);

  Object *call(std::list<Object *> arg_list, interp::LocalRuntime &r,
               interp::LexicalScope &s, bool);

  CFnType* type;
  ffi_cif fn_def;
  void (*fn_address)(void);
};

} // namespace expr
#endif // __HYDRA_FOREIGN_HPP
