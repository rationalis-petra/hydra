#include <string>
#include <iostream>

#include "expressions.hpp"

using std::string;
using std::to_string;


// compound types: string, array
hydra_t* hydra_t::singleton = nullptr;
hydra_t::hydra_t() {}
hydra_t* hydra_t::get() {
  if (singleton == nullptr) {
    singleton = new hydra_t;
    roots.insert(singleton);
  }
  return singleton;
}
string hydra_t::to_string() const {
  return "t";
}

hydra_string::hydra_string() {}
hydra_string::hydra_string(string str) {
  value = str;
}

string hydra_string::to_string() const {
  return '"' + value + '"';
}

string hydra_array::to_string() const {
  string out = "#A(";

  for (unsigned i = 0; i < array.size() ; i++) {
    out += array[i]->to_string();
    if (i != array.size() - 1)
      out += " ";
  }
  out += ")";
  return out;
}









// single-words: integer, characters
string hydra_num::to_string() const {
  return ::to_string(value);
}

hydra_num::hydra_num(int num) : value(num) {
}

hydra_char::hydra_char() : hydra_object() {}
hydra_char::hydra_char(int c) : value(c) {}
string hydra_char::to_string() const {
  return string("") + ((char) value);
}

// nil
hydra_nil::hydra_nil() {};
hydra_nil* hydra_nil::singleton = nullptr;
hydra_nil* hydra_nil::get() {
  if (singleton == nullptr) {
    singleton = new hydra_nil;
    roots.insert(singleton);
  }
  return singleton;
}
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
  if (stream != &std::cin) {
    delete stream;
  }
}
