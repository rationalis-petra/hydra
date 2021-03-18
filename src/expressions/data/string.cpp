
#include <string>
#include <typeinfo>

#include "expressions.hpp"

using namespace std;
using namespace expr;

Parent* HString::parent;

void HString::mark_node() {
  Object::mark_node();
}

HString::HString() {
  Symbol* pt = keyword_module->intern("parent");
  parents.insert(pt);
  slots[pt] = parent;
}
HString::HString(string str) {
  Symbol* pt = keyword_module->intern("parent");
  parents.insert(pt);
  slots[pt] = parent;
  value = str;
}

string HString::to_string() const {
  return '"' + value + '"';
}


