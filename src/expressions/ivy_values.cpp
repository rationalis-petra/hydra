#include "expressions.hpp"

using std::string;
using std::to_string;


// compound types: string, array
string ivy_t::to_string() const {
  return "t";
}

string ivy_string::to_string() const {
  return value;
}

string ivy_array::to_string() const {
  string out = "#A(";

  for (ivy_object* elt : array) {
    out += elt->to_string();
  }
  out += ")";
  return out;
}

// single-words: integer, characters
string ivy_num::to_string() const {
  return ::to_string(val);
}

string ivy_char::to_string() const {
  return string("") + value;
}

// nil
bool ivy_nil::null() const {
  return true;
}

string ivy_nil::to_string() const {
  return "nil";
}

// input stream
string ivy_istream::to_string() const {
  return "input stream";
}

ivy_istream::~ivy_istream() {
  delete stream;
}
