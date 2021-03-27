#ifndef __HYDRA_OBJECT_HPP
#define __HYDRA_OBJECT_HPP

#include <string>
#include <list>
#include <set>
#include <map>

#include "interpreter/garbage.hpp"
#include "interpreter/runtime.hpp"

namespace expr {

struct Operator;

struct Object {
  // CREATION & DELETION
  Object();
  virtual ~Object();

  // GARBAGE COLLECTION
  bool marked;
  virtual void mark_node();
  static interp::GarbageCollector* collector;

  std::set<Symbol*> parents;
  std::map<Symbol*, Object*> slots;
  std::map<Symbol*, Object*> metadata;
  Operator* invoker;

  // STANDARD FUNCTIONS
  virtual bool null() const;
  virtual std::string to_string(interp::LocalRuntime &r, interp::LexicalScope &s);
  virtual Object* eval(interp::LocalRuntime& r, interp::LexicalScope& s);

  // TYPING
  Object* derive_check(std::set<Object*> ptypes);

  // REFLECTION
  // the "get" and "set" functions provide hooks that mirror objects 
  // can use to access the object's slots, and 
  // virtual Object* mirror(interp::LocalRuntime &r, interp::LexicalScope &s);
  // virtual Object* get_slots(interp::LocalRuntime &r, interp::LexicalScope &s);
  // virtual Object* get_slot_value(interp::LocalRuntime &r, interp::LexicalScope &s);
  // virtual Object* set_slot_value(interp::LocalRuntime &r, interp::LexicalScope &s);
  // virtual Object* delete_slot(interp::LocalRuntime &r, interp::LexicalScope &s);

  // virtual Object* get_meta(interp::LocalRuntime &r, interp::LexicalScope &s);
  // virtual Object* get_meta_value(interp::LocalRuntime &r, interp::LexicalScope &s);
  // virtual Object* set_meta_value(interp::LocalRuntime &r, interp::LexicalScope &s);
  // virtual Object* delete_meta(interp::LocalRuntime &r, interp::LexicalScope &s);
  

};

}

#endif // __VALUE_HPP
