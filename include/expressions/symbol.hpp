#ifndef __HYDRA_EXPRESSIONS_SYMBOL
#define __HYDRA_EXPRESSIONS_SYMBOL

#include <map>
#include <set>
#include <string>

#include "interpreter/runtime.hpp"
#include "parent.hpp"
#include "object.hpp"

namespace expr {


// modules are like packages or namepsaces in other languages

struct Symbol : public Object {
  virtual void mark_node();
  static Parent* parent;

  // mutability
  bool mut;
  Object *value;
  std::string name;

  // std::list<hydra_module*> modules;

  Symbol(std::string name);
  static Symbol* symbol_no_parent(std::string name);
  std::string to_string(interp::LocalRuntime &r, interp::LexicalScope& s);
  Object* eval(interp::LocalRuntime &r, interp::LexicalScope &s);

private:
  Symbol();
};

}
#endif
