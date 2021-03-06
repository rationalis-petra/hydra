#include <string>
#include <istream>
#include <list>
#include <vector>

#include "object.hpp"

namespace expr {

struct Istream : public Object {
  Istream();
  static Parent* parent;
  virtual void mark_node();

  std::string to_string(interp::LocalRuntime &r, interp::LexicalScope& s);
  std::istream *stream;  
  ~Istream();
};

struct Ostream : public Object {
  Ostream();
  static Parent* parent;
  virtual void mark_node();

  std::string to_string(interp::LocalRuntime &r, interp::LexicalScope& s);
  std::ostream *stream;
  ~Ostream();
};

struct IOstream : public Object {
  IOstream();
  static Parent* parent;
  virtual void mark_node();

  std::string to_string(interp::LocalRuntime &r, interp::LexicalScope& s);
  std::iostream *stream;
  ~IOstream();
};

}

