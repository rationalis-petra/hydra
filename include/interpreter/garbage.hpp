#ifndef __HYDRA_INTERPRETER_GARBAGE_HPP
#define __HYDRA_INTERPRETER_GARBAGE_HPP

#include "interpreter/runtime.hpp"

namespace expr {
struct Object;
}

namespace interp {

struct LocalRuntime;

// A generic garbage collector class, in case
// I ever have multiple collector implementations
struct GarbageCollector {
  // self-explanatory
  virtual void collect_garbage(LocalRuntime &r) = 0;
  virtual void register_node(expr::Object* obj) = 0;

  virtual void register_context(LexicalScope* s) = 0;
  virtual void remove_context(LexicalScope* s) = 0;

  virtual void insert_root(expr::Object* obj) = 0;
  virtual void remove_root(expr::Object* obj) = 0;
};

  // A Concurrent stop-the-world garbage collector
struct STWCollector : public GarbageCollector {
  STWCollector(Runtime& r);
  void collect_garbage(LocalRuntime &r);
  void register_node(expr::Object* obj);

  void register_context(LexicalScope* s);
  void remove_context(LexicalScope* s);

  void insert_root(expr::Object* obj);
  void remove_root(expr::Object* obj);
  // A global Runtime
  Runtime& r;
  // Counts number of objects created since last call
  // to collect_garbage
  std::atomic<unsigned long> counter;

  // The list of all hydra objects
  std::list<expr::Object *> node_list;
  std::mutex node_list_mutex;

  // The list of root hydra objects - these are objects
  // which should not be deleted for reasons 'outside'
  // also a mutex to prevent concurrent access
  GarbageRoots roots;
  std::mutex root_mutex;

  // the list of all lexical contexts
  std::list<LexicalScope *> context_list;
  std::mutex context_mutex;
};

} // namespace interp
#endif
