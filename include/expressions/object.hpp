#ifndef __HYDRA_OBJECT_HPP
#define __HYDRA_OBJECT_HPP

#include <string>
#include <list>
#include <set>
#include <map>

#include "expressions/runtime.hpp"

namespace expr {

struct Object;

struct hydra_roots {
public:
  void remove(Object* obj);
  void insert(Object* obj);
  std::map<Object*, unsigned long> data;
};

struct Object {
  //HString docstring;

  std::set<Symbol*> parents;
  std::map<Symbol*, Object*> slots;

  virtual bool null() const;
  virtual std::string to_string() const = 0;
  virtual Object* eval(LocalRuntime& r, LexicalScope& s);
  Object* derive_check(std::set<Object*> ptypes);
  Object();
  virtual ~Object();


  // mark & sweep garbage collection
  // local variables/functions
  bool marked;
  virtual void mark_node() = 0;
  //virtual Object* clone() const = 0;

  // self-explanatory
  static void collect_garbage(LocalRuntime& r);

  // A global Runtime
  static Runtime r;
  // how many objects exist?
  static std::atomic<unsigned long> counter;
  // number of objects after most recent collection
  static unsigned long last;
  // The list of all hydra objects
  static std::list<Object*> node_list; 
  // The list of root hydra objects - these are objects
  // which should not be deleted for reasons 'outside'
  static hydra_roots roots; 
  // the list of all lexical contexts
  static std::list<LexicalScope*> context_list; 
};


std::ostream &operator<<(std::ostream &os, const Object *obj);

}

#endif // __VALUE_HPP
