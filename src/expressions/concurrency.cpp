#include <string>
#include <typeinfo>

#include "expressions.hpp"

using namespace std;
using namespace expr;

void threadstart(Operator* op, LocalRuntime* r, LexicalScope* s) {
  op->call({}, *r, *s);
}

Thread::Thread(Operator* op, LocalRuntime& r, LexicalScope& s) {
  // TODO: this leaks memory...
  LocalRuntime* loc = new LocalRuntime(r.r);
  LexicalScope* scop = new LexicalScope(s);
  thread = new std::thread(threadstart, op, loc, scop);
}

void Thread::mark_node() {
  marked = true;
};

string Thread::to_string() const {
  return "A thread handle";
}
