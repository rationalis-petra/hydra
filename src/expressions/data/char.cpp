#include <string>
#include <typeinfo>

#include "expressions.hpp"
#include "interpreter.hpp"

using namespace interp;
using namespace std;
using namespace expr;


Char::Char(int c) : value(c) {
  Symbol* pt = keyword_module->intern("parent");
  parents.insert(pt);
  slots[pt] = parent;
}

void Char::mark_node() { marked = true; }

string Char::to_string(LocalRuntime &r, LexicalScope &s) {
  return string("") + ((char)value);
}

Parent *Char::parent;
