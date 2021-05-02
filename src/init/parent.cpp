#include "expressions.hpp"
#include "init.hpp"

using namespace expr;

void init_parents() {
  // SETUP PARENTS
  // As hydra is prototype-based, objects of each "type" (integer, string, ...)
  // must notionally inherit from some parent object - the "string-parent" or
  // "integer parent", etc. We create them here.

  // Common-bechaviour and default-behaviour are two special cases:
  // common-behaviour acts a parent to almost all objects
  Object::common_behaviour = new Parent("Common Behaviour");
  Object::default_behaviour = new Parent("Default Behaviour");

  CType::parent = new Parent("ctype-parent");
  CType::modifier_parent = new Parent("ctypemod-parent");
  //CProxy::parent = new Parent("cproxy-parent");

  // Here are the rest of the "normal" types
  Number::parent = new Parent("number-parent");
  Complex::parent = new Parent("complex-parent");
  Real::parent = new Parent("real-parent");
  Integer::parent = new Parent("integer-parent");
  Float::parent = new Parent("float-parent");

  HString::parent = new Parent("string-parent");
  Mutex::parent = new Parent("mutex-parent");
  Cons::parent = new Parent("cons-parent");
  Tuple::parent = new Parent("tuple-parent");
  Char::parent = new Parent("char-parent");
  Union::parent = new Parent("union-parent");
  Mirror::parent = new Parent("mirror-parent");
  Module::parent = new Parent("module-parent");
  Symbol::parent = new Parent("symbol-parent");

  Istream::parent = new Parent("istream-parent");
  Ostream::parent = new Parent("ostream-parent");
  IOstream::parent = new Parent("iostream-parent");
  Socket::parent = new Parent("socket-parent");
  Acceptor::parent = new Parent("acceptor-parent");
}
