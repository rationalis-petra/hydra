#ifndef __HYDRA_OBJECT_HPP
#define __HYDRA_OBJECT_HPP

#include <string>
#include <list>
#include <set>
#include <map>

struct runtime;
struct lexical_scope;
struct hydra_object;

struct hydra_roots {
public:
  void remove(hydra_object* obj);
  void insert(hydra_object* obj);
  std::map<hydra_object*, unsigned long> data;
};

struct hydra_object {
  //std::string docstring;

  virtual bool null() const;
  virtual std::string to_string() const = 0;
  virtual hydra_object* eval(runtime& r, lexical_scope& s);
  hydra_object();
  virtual ~hydra_object();

  // mutability
  bool mut;

  // mark & sweep garbage collection
  // local variables/functions
  bool marked;
  virtual void mark_node() = 0;

  //static runtime *r;
  // how many objects exist?
  static unsigned long counter;
  // number of objects after most recent collection
  static unsigned long last;
  // The list of all hydra objects
  static std::list<hydra_object*> node_list; 
  // The list of root hydra objects - these are objects
  // which should not be deleted for reasons 'outside'
  static hydra_roots roots; 

  // the list of all lexical contexts
  static std::list<lexical_scope*> context_list; 
  // self-explanatory
  static void collect_garbage(runtime& r);
};


std::ostream &operator<<(std::ostream &os, const hydra_object *obj);

#endif // __HYDRA_OBJECT_HPP
