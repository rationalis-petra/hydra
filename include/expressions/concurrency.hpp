#include <string>

#include "expressions/object.hpp"

namespace expr {

struct Thread : public Value {
  void mark_node();
  std::string to_string() const;
};

struct Mutex : public Value {
  void mark_node();
  std::string to_string() const;
};

struct Semaphore : public Value {
  void mark_node();
  std::string to_string() const;
};

}
