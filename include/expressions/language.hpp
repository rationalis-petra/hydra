#ifndef __HYDRA_EXPRESSIONS_LANGUAGE
#define __HYDRA_EXPRESSIONS_LANGUAGE

#include <map>
#include <set>
#include <string>

#include "object.hpp"

namespace expr {

struct Symbol;
struct Vector;

// modules are like packages or namepsaces in other languages
struct Module : public Object {
  virtual void mark_node();

  Module();
  Module(std::string name);
  std::string to_string(LocalRuntime &r, LexicalScope& s);
  std::map<std::string, Symbol *> symbols;
  std::map<std::string, Symbol *> exported_symbols;

  std::string name;

  Object *get(std::string str);
  Object *get(std::list<std::string> str);
  Symbol *intern(std::string str);
  Symbol *intern(std::list<std::string> str);
};

struct Symbol : public Object {
  virtual void mark_node();

  // mutability
  bool mut;
  Object *value;
  std::string name;

  // std::list<hydra_module*> modules;

  Symbol(std::string name);
  std::string to_string(LocalRuntime &r, LexicalScope& s);
  Object *eval(LocalRuntime &r, LexicalScope &s);
};

struct Pattern : public Object {
  virtual void mark_node();
  std::string to_string(LocalRuntime &r, LexicalScope& s);

  Object* match(Object* o);
  Object *eval(LocalRuntime &r, LexicalScope &s);
};

struct Ref : public Object {
  std::string to_string(LocalRuntime &r, LexicalScope& s);
  virtual void mark_node();

  Object *ptr;
};

}
#endif
