#ifndef __HYDRA_EXPRESSIONS_MODULE
#define __HYDRA_EXPRESSIONS_MODULE

#include <map>
#include <set>
#include <string>

#include "interpreter/runtime.hpp"
#include "object.hpp"
#include "parent.hpp"

namespace expr {

struct Module : public Object {
  static Parent *parent;

  virtual Object *get(std::string str) = 0;
  virtual Object *get(std::list<std::string> str) = 0;
  virtual Symbol *intern(std::string str) = 0;
  virtual Symbol *intern(std::list<std::string> str) = 0;

  virtual void insert(Symbol* sym) = 0;
  virtual void remove(std::string) = 0;
  virtual void export_sym(std::string) = 0;
  virtual Object *get_exported_symbols() = 0;

  virtual std::string get_name() = 0;
};

struct ModuleImpl : public Module {
  virtual void mark_node();

  static ModuleImpl* no_parent(std::string name);
  ModuleImpl(std::string name);

  Object *get(std::string str);
  Object *get(std::list<std::string> str);
  Symbol *intern(std::string str);
  Symbol *intern(std::list<std::string> str);

  void insert(Symbol* sym);
  void remove(std::string);
  void export_sym(std::string);
  Object *get_exported_symbols();

  void set_parent();
  std::string get_name();

private: 
  ModuleImpl();

  std::string to_string(interp::LocalRuntime &r, interp::LexicalScope &s);
  std::map<std::string, Symbol *> symbols;
  std::map<std::string, Symbol *> exported_symbols;

  std::string name;
};

} // namespace expr

#endif
