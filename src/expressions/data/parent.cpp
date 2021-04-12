#include <string>
#include <typeinfo>

#include "expressions.hpp"

using namespace std;
using namespace expr;
using namespace interp;

string Parent::to_string(LocalRuntime &r, LexicalScope &s) {
  return name;
}

Parent::Parent(string _name) : name(_name)  {}
