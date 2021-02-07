#include <string>

#include "expressions/object.hpp"


struct hydra_thread : public hydra_object {
  virtual void mark_node();
  std::string to_string() const;
};

struct hydra_mutex : public hydra_object {
  virtual void mark_node();
  std::string to_string() const;
};

struct hydra_semaphore : public hydra_object {
  virtual void mark_node();
  std::string to_string() const;
};
