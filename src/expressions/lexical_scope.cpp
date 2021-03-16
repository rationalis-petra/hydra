#include <iostream>
#include "expressions.hpp"

using namespace expr;
using std::mutex;
using std::unique_lock;

mutex Object::context_mutex;

LexicalScope::LexicalScope() {
  unique_lock<mutex> lck(Object::context_mutex);
  Object::context_list.push_front(this);
  lck.unlock();
}

LexicalScope::~LexicalScope() {
  unique_lock<mutex> lck(Object::context_mutex);
  Object::context_list.remove(this);
  lck.unlock();
}
