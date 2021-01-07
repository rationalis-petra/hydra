#include <string>

#include "expressions.hpp"

using std::string;
using std::to_string;


// compound types: string, array
string hydra_t::to_string() const {
  return "t";
}

string hydra_string::to_string() const {
  return value;
}

string hydra_array::to_string() const {
  string out = "#A(";

  for (hydra_object* elt : array) {
    out += elt->to_string();
  }
  out += ")";
  return out;
}

// single-words: integer, characters
string hydra_num::to_string() const {
  return ::to_string(val);
}

string hydra_char::to_string() const {
  return string("") + value;
}

// nil
bool hydra_nil::null() const {
  return true;
}

string hydra_nil::to_string() const {
  return "nil";
}

// input stream
string hydra_istream::to_string() const {
  return "input stream";
}

hydra_istream::~hydra_istream() {
  delete stream;
}
