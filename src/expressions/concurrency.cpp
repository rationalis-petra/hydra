#include <string>
#include <typeinfo>

#include "expressions.hpp"

using namespace std;
using namespace expr;

void threadstart(Operator* op, std::list<Object*> args, LocalRuntime* r, LexicalScope* s) {
  op->call(args, *r, *s);
}

Thread::Thread(Operator* op, std::list<Object*> args, LocalRuntime& r, LexicalScope& s) {
  // TODO: this leaks memory...
  // how?
  LocalRuntime* loc = new LocalRuntime(r.r);
  LexicalScope* scop = new LexicalScope(s);
  thread = new std::thread(threadstart, op, args, loc, scop);
}

void Thread::mark_node() {
  Object::mark_node();
};

string Thread::to_string(LocalRuntime &r, LexicalScope &s) {
  return "A thread handle";
}
