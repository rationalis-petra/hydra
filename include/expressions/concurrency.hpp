#include <string>
#include <thread>
#include <mutex>

#include "expressions/object.hpp"

namespace expr {

struct Operator;

struct Thread : public Object {
  Thread(Operator* op, std::list<Object*> args, interp::LocalRuntime& r, interp::LexicalScope& s);

  void mark_node();
  std::string to_string(interp::LocalRuntime &r, interp::LexicalScope& s);
  std::thread* thread;
};

struct Mutex : public Object {
  Mutex();
  static Parent* parent;
  std::string to_string(interp::LocalRuntime &r, interp::LexicalScope& s);
  std::mutex mtx;

  void lock();
  void unlock();
};

// struct Semaphore : public Object {
//   void mark_node();
//   std::string to_string() const;
// };

}
