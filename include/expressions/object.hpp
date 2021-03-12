#ifndef __HYDRA_OBJECT_HPP
#define __HYDRA_OBJECT_HPP

#include <string>
#include <list>
#include <set>
#include <map>


namespace expr {

struct LocalRuntime;
struct LexicalScope;
struct Value;

struct hydra_roots {
public:
  void remove(Value* obj);
  void insert(Value* obj);
  std::map<Value*, unsigned long> data;
};

struct Value {
  //std::string docstring;

  virtual bool null() const;
  virtual std::string to_string() const = 0;
  virtual Value* eval(LocalRuntime& r, LexicalScope& s);
  Value();
  virtual ~Value();


  // mark & sweep garbage collection
  // local variables/functions
  bool marked;
  virtual void mark_node() = 0;
  //virtual Value* copy() const = 0;

  // how many objects exist?
  static unsigned long counter;
  // number of objects after most recent collection
  static unsigned long last;
  // The list of all hydra objects
  static std::list<Value*> node_list; 
  // The list of root hydra objects - these are objects
  // which should not be deleted for reasons 'outside'
  static hydra_roots roots; 

  // the list of all lexical contexts
  static std::list<LexicalScope*> context_list; 
  // self-explanatory
  static void collect_garbage(LocalRuntime& r);
};


std::ostream &operator<<(std::ostream &os, const Value *obj);

}

#endif // __VALUE_HPP
