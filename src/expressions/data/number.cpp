
#include <string>
#include <typeinfo>

#include "expressions.hpp"

using namespace std;
using namespace expr;

Parent* Integer::parent;

string Integer::to_string(LocalRuntime &r, LexicalScope &s) {
  return std::to_string(value);
}

Integer::Integer(int num) : value(num) {
  Symbol* pt = keyword_module->intern("parent");
  parents.insert(pt);
  slots[pt] = parent;
}

