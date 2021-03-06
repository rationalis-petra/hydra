#include <string>
#include <iostream>

#include "expressions.hpp"

using std::string;
using std::to_string;

/// TRUE

// compound types: string, array
void hydra_t::mark_node() {
  marked = true;
}

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



//// STRING

void hydra_string::mark_node() {
  marked = true;
}

hydra_string::hydra_string() {}
hydra_string::hydra_string(string str) {
  value = str;
}

string hydra_string::to_string() const {
  return '"' + value + '"';
}



/// ARRAY

void hydra_vector::mark_node() {
  marked = true;
  for (hydra_object* o : array) {
    o->mark_node();
  }
}
string hydra_vector::to_string() const {
  string out = "[@v ";

  for (unsigned i = 0; i < array.size() ; i++) {
    out += array[i]->to_string();
    if (i != array.size() - 1)
      out += " ";
  }
  out += "]";
  return out;
}


/// TUPLE

void hydra_tuple::mark_node() {
  marked = true;
  for (hydra_object* o : values) {
    o->mark_node();
  }
}
string hydra_tuple::to_string() const {
  string out = "[@t ";

  for (unsigned i = 0; i < values.size() ; i++) {
    out += values[i]->to_string();
    if (i != values.size() - 1)
      out += " ";
  }
  out += "]";
  return out;
}

/// UNION

void hydra_union::mark_node() {
  marked = true;
  tag->mark_node();
  value->mark_node();
}

string hydra_union::to_string() const {
  string out = "[";
  out += tag->to_string();
  out += " ";
  out += value->to_string();
  out += "]";
  return out;
}


//// NUMBER



// single-words: integer, characters
void hydra_num::mark_node() {
  marked = true;
}
string hydra_num::to_string() const {
  return ::to_string(value);
}

hydra_num::hydra_num(int num) : value(num) {}

//// CHARACTER
void hydra_char::mark_node() {
  marked = true;
}
hydra_char::hydra_char() : hydra_object() {}
hydra_char::hydra_char(int c) : value(c) {}
string hydra_char::to_string() const {
  return string("") + ((char) value);
}

/// NIL

void hydra_nil::mark_node() {
  marked = true;
}

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


/* STREAMS */
// INPUT STREAM
void hydra_istream::mark_node() {
  marked = true;
}

string hydra_istream::to_string() const {
  return "input stream";
}

hydra_istream::~hydra_istream() {
  if (stream != &std::cin) {
    delete stream;
  }
}

// OUTPUT STREAM
void hydra_ostream::mark_node() {
  marked = true;
}

string hydra_ostream::to_string() const {
  return "output stream";
}

hydra_ostream::~hydra_ostream() {
  if (stream != &std::cout) {
    delete stream;
  }
}

// INPUT-OUTPUT STREAM
void hydra_iostream::mark_node() {
  marked = true;
}

string hydra_iostream::to_string() const {
  return "input/output stream";
}

hydra_iostream::~hydra_iostream() {
  delete stream;
}

void hydra_ref::mark_node() {
  if (marked) return;
  marked = true;
  ptr->mark_node();
}

string hydra_ref::to_string() const{
  return "[ref " + ptr->to_string() + "]";
}

void hydra_var::mark_node() {
  if (marked) return;
  marked = true;
  val->mark_node();
}

string hydra_var::to_string() const{
  return "[var " + val->to_string() + "]";
}
