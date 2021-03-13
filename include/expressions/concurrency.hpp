#include <string>

#include "expressions/object.hpp"

namespace expr {

struct Thread : public Object {
  void mark_node();
  std::string to_string() const;
};

struct Mutex : public Object {
  void mark_node();
  std::string to_string() const;
};

struct Semaphore : public Object {
  void mark_node();
  std::string to_string() const;
};

}
