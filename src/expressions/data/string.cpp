
#include <string>
#include <typeinfo>

#include "expressions.hpp"

using namespace std;
using namespace expr;

Parent* HString::parent;

void HString::mark_node() {
  Object::mark_node();
}

HString::HString() {}
HString::HString(string str) {
  value = str;
}

string HString::to_string() const {
  return '"' + value + '"';
}
