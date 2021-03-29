#include <string>
#include <typeinfo>

#include "expressions.hpp"
#include "interpreter.hpp"
#include "utils.hpp"

using namespace interp;
using namespace expr;

using std::string;

Mirror::Mirror(Object *_reflectee) : reflectee(_reflectee) {
  Symbol* pt = keyword_module->intern("parent");
  parents.insert(pt);
  slots[pt] = parent;
}

string Mirror::to_string(LocalRuntime& r, LexicalScope& s) {
  return "Mirror " + hydra_to_string(reflectee, r, s);
}

void Mirror::mark_node() {
  if (marked) return;
  Object::mark_node();
  reflectee->mark_node();
}

Object *Mirror::parent;
