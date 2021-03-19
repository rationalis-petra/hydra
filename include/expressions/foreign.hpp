#ifndef __HYDRA_EXPRESSIONS_FOREIGN_HPP
#define __HYDRA_EXPRESSIONS_FOREIGN_HPP

#include <list>
#include <string>

#include "ltdl.h"
#include "ffi.h"

#include "types.hpp"
#include "object.hpp"

namespace expr {

enum foreign_type { Int32, Pointer, String, Void };

struct ForeignLib : public Object {
  virtual void mark_node();

  ForeignLib(lt_dlhandle lib);
  std::string to_string(LocalRuntime &r, LexicalScope& s);

  lt_dlhandle lib;
};

struct ForeignSymbol : public Object {
  virtual void mark_node();

  ForeignSymbol(void *addr);
  std::string to_string(LocalRuntime &r, LexicalScope& s);

  void* address;
};

struct ForeignOperator : public Operator {
  ForeignOperator();

  Object* call(std::list<Object*> arg_list, LocalRuntime& r, LexicalScope &s, bool);

  std::list<foreign_type> arg_types;
  foreign_type return_type;
  ffi_cif fn_def;
  void(*fn_address)(void);
};

}
#endif // __HYDRA_FOREIGN_HPP
