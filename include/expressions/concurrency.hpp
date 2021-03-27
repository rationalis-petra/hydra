#include <string>
#include <thread>

#include "expressions/object.hpp"

namespace expr {

struct Operator;

struct Thread : public Object {
  Thread(Operator* op, std::list<Object*> args, interp::LocalRuntime& r, interp::LexicalScope& s);

  void mark_node();
  std::string to_string(interp::LocalRuntime &r, interp::LexicalScope& s);
  std::thread* thread;
};

// struct Mutex : public Object {
//   void mark_node();
//   std::string to_string() const;
// };

// struct Semaphore : public Object {
//   void mark_node();
//   std::string to_string() const;
// };

}
