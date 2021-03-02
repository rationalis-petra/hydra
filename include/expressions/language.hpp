#ifndef __HYDRA_EXPRESSIONS_LANGUAGE
#define __HYDRA_EXPRESSIONS_LANGUAGE

#include <map>
#include <set>
#include <string>

#include "object.hpp"

struct hydra_symbol;
struct type_fn;
struct hydra_string;

// modules are like packages or namepsaces in other languages
struct hydra_module : public hydra_object {
  virtual void mark_node();

  hydra_module();
  hydra_module(std::string name);
  std::string to_string() const;
  std::map<std::string, hydra_symbol *> symbols;
  std::map<std::string, hydra_symbol *> exported_symbols;

  std::string name;

  hydra_object *get(std::string str);
  hydra_object *get(std::list<std::string> str);
  hydra_symbol *intern(std::string str);
  hydra_symbol *intern(std::list<std::string> str);
};

struct hydra_symbol : public hydra_object {
  virtual void mark_node();

  hydra_object *value;
  std::string name;

  // std::list<hydra_module*> modules;

  hydra_symbol(std::string name);
  std::string to_string() const;
  hydra_object *eval(runtime &r, lexical_scope &s);
};

struct hydra_pattern : public hydra_object {
  virtual void mark_node();
  std::string to_string() const;

  hydra_object* match(hydra_object* o);
  hydra_object *eval(runtime &r, lexical_scope &s);
};

#endif
