#ifndef __HYDRA_EXPRESSIONS_RUNTIME_HPP
#define __HYDRA_EXPRESSIONS_RUNTIME_HPP

#include <list>
#include <unordered_map>
#include <vector>
#include <thread>
#include <atomic>
#include <set>
#include <condition_variable>

namespace expr {
  struct Object;
  struct Module;
  struct Operator;
  struct Symbol;
  
  struct condition_handler;
  struct hydra_restart;
  
} // namespace expr

namespace interp {

struct GarbageRoots {

public:
  void remove(expr::Object* obj);
  void insert(expr::Object* obj);
  std::unordered_map<expr::Object*, unsigned long> data;
};

struct LocalRuntime;

struct Runtime {
  expr::Module* root;
  // the set of all active threads and associated lock
  std::set<std::thread> thread_pool;

  std::unordered_map<char, expr::Operator*> readtable;
  std::vector<LocalRuntime*> local_runtimes;

  // this is for the garbage collector. for more details,
  // see src/memory/garbage.cpp
  bool collecting;
  std::condition_variable collect; 
  std::mutex collect_mutex;
  std::condition_variable finished; 
  std::mutex finished_mutex;
};

// A runtime containing thread-local variables
struct LocalRuntime {
  LocalRuntime(Runtime& r);
  Runtime& r;
  std::list<expr::condition_handler*> handlers;
  std::list<expr::hydra_restart*> restarts;
  expr::Module* active_module;


  // a thread will set this to true when it is waiting for garbage to be 
  // collected, and the garbage collector will set it to false when it is
  // done, so the thread may continue. 
  bool can_collect;
  GarbageRoots roots; 
  static std::mutex root_mutex;
};

// the lexical scope contians a mapping of symbols to their local values
struct LexicalScope {
  std::unordered_map<expr::Symbol*, expr::Object*> map;
  LexicalScope();
  ~LexicalScope();
};

}

#endif
