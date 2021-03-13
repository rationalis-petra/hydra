#ifndef __HYDRA_EXPRESSIONS_RUNTIME_HPP
#define __HYDRA_EXPRESSIONS_RUNTIME_HPP

#include <list>
#include <map>
#include <vector>

namespace expr {

struct Value;
struct Module;
struct Operator;
struct Symbol;

struct condition_handler;
struct hydra_restart;

struct LocalRuntime;

struct Runtime {
  Module* root;
  std::map<char, Operator*> readtable;
  std::vector<LocalRuntime*> local_runtimes;
};

// A runtime containing thread-local variables
struct LocalRuntime {
  LocalRuntime(Runtime& r);
  Runtime& r;
  std::list<condition_handler*> handlers;
  std::list<hydra_restart*> restarts;
  Module* active_module;
};

// the lexical scope contians a mapping of symbols to their local values
struct LexicalScope {
  std::map<Symbol*, Value*> map;
  LexicalScope();
  ~LexicalScope();
};

}

#endif
