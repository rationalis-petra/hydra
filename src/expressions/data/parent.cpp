#include <string>
#include <typeinfo>

#include "expressions.hpp"
#include "utils.hpp"

using namespace std;
using namespace expr;
using namespace interp;

string Parent::to_string(LocalRuntime &r, LexicalScope &s) {
  return name;
}

Parent::Parent(string _name) : name(_name)  {}

void Parent::set_parent(std::string name, Object *value) {
  Symbol* sym = get_keyword(name);
  slots[sym] = value;
  parents.insert(sym);
}
