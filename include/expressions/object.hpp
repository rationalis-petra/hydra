#ifndef __HYDRA_OBJECT_HPP
#define __HYDRA_OBJECT_HPP
#include <list>
#include <map>
#include <set>
#include <string>

#include "interpreter/garbage.hpp"
#include "interpreter/runtime.hpp"
#include "interpreter/continuation.hpp"

namespace expr {

struct Operator;
struct Parent;
struct Contract;

struct Object {
  // PARENTS
  static Parent* default_behaviour;
  static Parent* common_behaviour;

  // CREATION & DELETION
  Object();
  virtual ~Object();

  // GARBAGE COLLECTION
  bool marked;
  virtual void mark_node();
  static interp::GarbageCollector *collector;

  std::set<Symbol *> parents;
  std::map<Symbol *, Object *> slots;
  std::map<Symbol *, Object *> metadata;
  Contract* contract;
  Operator *invoker;

  // STANDARD FUNCTIONS
  virtual bool null() const;
  virtual std::string to_string(interp::LocalRuntime &r,
                                interp::LexicalScope &s);
  virtual Object *eval(interp::LocalRuntime &r, interp::LexicalScope &s);

  // TYPING
  Object *derive_check(std::set<Object *> ptypes, std::set<Object*> visited);

  // REFLECTION
  // the "get" and "set" functions provide hooks that mirror objects
  // can use to access the object's slots, and
  virtual Object *reflect(interp::LocalRuntime &r, interp::LexicalScope &s);
  virtual Object *get_slots(interp::LocalRuntime &r, interp::LexicalScope &s);
  virtual Object *get_slot_value(Symbol *sym, interp::LocalRuntime &r, interp::LexicalScope &s);
  virtual Object *set_slot_value(Symbol *sym, Object *val, interp::LocalRuntime &r, interp::LexicalScope &s);
  virtual Object *delete_slot(Symbol *sym, interp::LocalRuntime &r, interp::LexicalScope &s);

  virtual Object *get_meta(interp::LocalRuntime &r, interp::LexicalScope &s);
  virtual Object *get_meta_value(Symbol *sym, interp::LocalRuntime &r,
                                 interp::LexicalScope &s);
  virtual Object *set_meta_value(Symbol *sym, Object *val,
                                 interp::LocalRuntime &r,
                                 interp::LexicalScope &s);
  virtual Object *delete_meta(Symbol *sym, interp::LocalRuntime &r,
                              interp::LexicalScope &s);

};

} // namespace expr

#endif // __VALUE_HPP
