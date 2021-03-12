
#include <string>

#include "expressions.hpp"
#include "types.hpp"

using std::string;
using std::list;

using namespace type;

// INPUT-OUTPUT STREAM
void IOStream::mark_node() {
  marked = true;
}

string IOStream::to_string() const {
  return "IOStream";
}

expr::Value *IOStream::check_type(expr::Value* obj) {
  if ((dynamic_cast<expr::IOstream*>(obj)) == nullptr) {
    return expr::nil::get();
  } else {
    return expr::t::get();
  }
}


// INPUT STREAM
void Istream::mark_node() {
  marked = true;
}

string Istream::to_string() const {
  return "IStream";
}

expr::Value *Istream::check_type(expr::Value* obj) {
  if ((dynamic_cast<expr::Istream*>(obj)) == nullptr &&
      (dynamic_cast<expr::IOstream*>(obj)) == nullptr) {
    return expr::nil::get();
  } else {
    return expr::t::get();
  }
}


// OUTPUT STREAM
void Ostream::mark_node() {
  marked = true;
}

string Ostream::to_string() const {
  return "OStream";
}

expr::Value *Ostream::check_type(expr::Value* obj) {
  if ((dynamic_cast<expr::Ostream*>(obj)) == nullptr &&
      (dynamic_cast<expr::IOstream*>(obj)) == nullptr) {
    return expr::nil::get();
  } else {
    return expr::t::get();
  }
}

