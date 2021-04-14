#ifndef __HYDRA_EXPRESSIONS_LANGUAGE
#define __HYDRA_EXPRESSIONS_LANGUAGE

#include <map>
#include <set>
#include <string>

#include "interpreter/runtime.hpp"
#include "parent.hpp"
#include "object.hpp"

namespace expr {

struct Symbol;
struct Vector;

// modules are like packages or namepsaces in other languages
struct Module : public Object {
  virtual void mark_node();

  Module();
  Module(std::string name);
  std::string to_string(interp::LocalRuntime &r, interp::LexicalScope& s);
  std::map<std::string, Symbol *> symbols;
  std::map<std::string, Symbol *> exported_symbols;
  void set_parent();

  static Parent* parent;

  std::string name;

  Object *get(std::string str);
  Object *get(std::list<std::string> str);
  Symbol *intern(std::string str);
  Symbol *intern(std::list<std::string> str);
};

struct Symbol : public Object {
  virtual void mark_node();
  static Parent* parent;

  // mutability
  bool mut;
  Object *value;
  std::string name;

  // std::list<hydra_module*> modules;

  Symbol(std::string name);
  static Symbol* symbol_no_parent(std::string name);
  std::string to_string(interp::LocalRuntime &r, interp::LexicalScope& s);
  Object *eval(interp::LocalRuntime &r, interp::LexicalScope &s);

private:
  Symbol();
};

struct Pattern : public Object {
  virtual void mark_node();
  std::string to_string(interp::LocalRuntime &r, interp::LexicalScope& s);

  Object* match(Object* o);
  Object *eval(interp::LocalRuntime &r, interp::LexicalScope &s);
};

}
#endif
