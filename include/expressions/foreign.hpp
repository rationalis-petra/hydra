#ifndef __HYDRA_EXPRESSIONS_FOREIGN_HPP
#define __HYDRA_EXPRESSIONS_FOREIGN_HPP

#include "expressions/module.hpp"

#include <list>
#include <string>

#include "ffi.h"
#include "ltdl.h"

#include "object.hpp"
#include "types.hpp"

namespace expr {

struct CFnType;

struct ForeignLib : public Module {
  virtual void mark_node();

  std::unordered_map<std::string, Symbol*> symbols;
  std::set<Symbol*> exports;
  std::string name;

  Object *get(std::string str);
  Object *get(std::list<std::string> str);
  Symbol *intern(std::string str);
  Symbol *intern(std::list<std::string> str);

  void insert(Symbol* sym);
  void remove(std::string);
  void export_sym(std::string);
  Object *get_exported_symbols();

  std::string get_name();

  ForeignLib(lt_dlhandle lib);
  std::string to_string(interp::LocalRuntime &r, interp::LexicalScope &s);

  lt_dlhandle lib;
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
