#include <string>
#include <thread>

#include "expressions/object.hpp"

namespace expr {

struct Operator;
struct LocalRuntime;
struct LexicalScope;

struct Thread : public Object {
  Thread(Operator* op, LocalRuntime& r, LexicalScope& s);

  void mark_node();
  std::string to_string() const;
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
