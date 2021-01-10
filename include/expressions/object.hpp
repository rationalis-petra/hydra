#ifndef __HYDRA_OBJECT_HPP
#define __HYDRA_OBJECT_HPP

#include <string>
#include <list>

struct runtime;

struct hydra_object {
  //std::string docstring;

  virtual bool null() const;
  virtual std::string to_string() const = 0;
  virtual hydra_object* eval(runtime& r);
  hydra_object();
  virtual ~hydra_object();


  // mark & sweep garbage collection
  bool marked;
  static std::list<hydra_object*> node_list; 
  static void collect_garbage(runtime& r);
};


std::ostream &operator<<(std::ostream &os, const hydra_object *obj);

#endif // __HYDRA_OBJECT_HPP
