#include <string>
#include <typeinfo>

#include "expressions.hpp"
#include "utils.hpp"

using namespace std;
using namespace expr;
using namespace interp;

Parent* Mutex::parent;

Mutex::Mutex() {
  Symbol* pt = get_keyword("parent");
  parents.insert(pt);
  slots[pt] = parent;
}

string Mutex::to_string(LocalRuntime &r, LexicalScope &s) {
  return "<standard mutex>";
}

void Mutex::lock() {
  mtx.lock();
}

void Mutex::unlock() {
  mtx.unlock();
}
