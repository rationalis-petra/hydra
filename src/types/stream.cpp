
#include <string>

#include "expressions.hpp"
#include "types.hpp"

using std::string;

using namespace type;
using namespace interp;

// INPUT-OUTPUT STREAM
void IOStream::mark_node() {
  Object::mark_node();
}

string IOStream::to_string(LocalRuntime &r, LexicalScope &s) {
  return "IOStream";
}

expr::Object *IOStream::check_type(expr::Object* obj) {
  if ((dynamic_cast<expr::IOstream*>(obj)) == nullptr) {
    return expr::nil::get();
  } else {
    return expr::t::get();
  }
}

expr::Object *IOStream::subtype(Type *ty) {
  if (dynamic_cast<IOStream *>(ty)) {
    return expr::t::get();
  } else {
    return expr::nil::get();
  }
}

// INPUT STREAM
void Istream::mark_node() {
  marked = true;
}

string Istream::to_string(LocalRuntime &r, LexicalScope &s) {
  return "IStream";
}

expr::Object *Istream::check_type(expr::Object* obj) {
  if ((dynamic_cast<expr::Istream*>(obj)) == nullptr &&
      (dynamic_cast<expr::IOstream*>(obj)) == nullptr) {
    return expr::nil::get();
  } else {
    return expr::t::get();
  }
}

expr::Object *Istream::subtype(Type *ty) {
  if (dynamic_cast<IOStream *>(ty) || dynamic_cast<Istream*>(ty)) {
    return expr::t::get();
  } else {
    return expr::nil::get();
  }
}

// OUTPUT STREAM
void Ostream::mark_node() {
  marked = true;
}

string Ostream::to_string(LocalRuntime &r, LexicalScope &s) {
  return "OStream";
}

expr::Object *Ostream::check_type(expr::Object* obj) {
  if ((dynamic_cast<expr::Ostream*>(obj)) == nullptr &&
      (dynamic_cast<expr::IOstream*>(obj)) == nullptr) {
    return expr::nil::get();
  } else {
    return expr::t::get();
  }
}

expr::Object *Ostream::subtype(Type *ty) {
  if (dynamic_cast<IOStream *>(ty) || dynamic_cast<Ostream*>(ty)) {
    return expr::t::get();
  } else {
    return expr::nil::get();
  }
}
