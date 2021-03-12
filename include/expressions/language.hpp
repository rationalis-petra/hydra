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
struct Module : public Value {
  virtual void mark_node();

  Module();
  Module(std::string name);
  std::string to_string() const;
  std::map<std::string, Symbol *> symbols;
  std::map<std::string, Symbol *> exported_symbols;

  std::string name;

  Value *get(std::string str);
  Value *get(std::list<std::string> str);
  Symbol *intern(std::string str);
  Symbol *intern(std::list<std::string> str);
};

struct Symbol : public Value {
  virtual void mark_node();

  // mutability
  bool mut;
  Value *value;
  std::string name;

  // std::list<hydra_module*> modules;

  Symbol(std::string name);
  std::string to_string() const;
  Value *eval(LocalRuntime &r, LexicalScope &s);
};

struct Pattern : public Value {
  virtual void mark_node();
  std::string to_string() const;

  Value* match(Value* o);
  Value *eval(LocalRuntime &r, LexicalScope &s);
};

struct Ref : public Value {
  std::string to_string() const;
  virtual void mark_node();

  Value *ptr;
};

}
#endif
