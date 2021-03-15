#include <string>
#include <iostream>

#include "expressions.hpp"

using std::string;
using std::to_string;

using namespace expr;

/// TRUE

// compound types: string, array
void t::mark_node() {
  marked = true;
}

t* t::singleton = nullptr;
t::t() {}
t* t::get() {
  if (singleton == nullptr) {
    singleton = new t;
    roots.insert(singleton);
  }
  return singleton;
}

string t::to_string() const {
  return "t";
}



//// STRING

void HString::mark_node() {
  marked = true;
}

HString::HString() {}
HString::HString(string str) {
  value = str;
}

string HString::to_string() const {
  return '"' + value + '"';
}



/// Vector

unsigned Vector::size() {
  return array.size();
}

Object* Vector::operator[](int i) {
  return array[i];
}

void Vector::mark_node() {
  marked = true;
  for (Object* o : array) {
    o->mark_node();
  }
}

string Vector::to_string() const {
  string out = "(vector ";

  for (unsigned i = 0; i < array.size() ; i++) {
    out += array[i]->to_string();
    if (i != array.size() - 1)
      out += " ";
  }
  out += ")";
  return out;
}


/// TUPLE

unsigned Tuple::size() {
  return values.size();
}

Object* Tuple::operator[](int i) {
  return values[i];
}

void Tuple::mark_node() {
  marked = true;
  for (Object* o : values) {
    o->mark_node();
  }
}

string Tuple::to_string() const {
  string out = "[";

  for (unsigned i = 0; i < values.size() ; i++) {
    out += values[i]->to_string();
    if (i != values.size() - 1)
      out += " ";
  }
  out += "]";
  return out;
}

/// UNION

void Union::mark_node() {
  marked = true;
  tag->mark_node();
  value->mark_node();
}

string Union::to_string() const {
  string out = "(Sum ";
  out += tag->to_string();
  out += " ";
  out += value->to_string();
  out += ")";
  return out;
}


//// NUMBER



// single-words: integer, characters
void Integer::mark_node() {
  marked = true;
}
string Integer::to_string() const {
  return ::to_string(value);
}

Integer::Integer(int num) : value(num) {}

//// CHARACTER
void Char::mark_node() {
  marked = true;
}
Char::Char() : Object() {}
Char::Char(int c) : value(c) {}
string Char::to_string() const {
  return string("") + ((char) value);
}

/// NIL

void nil::mark_node() {
  marked = true;
}

nil::nil() {};
nil* nil::singleton = nullptr;
nil* nil::get() {
  if (singleton == nullptr) {
    singleton = new nil;
    roots.insert(singleton);
  }
  return singleton;
}
bool nil::null() const {
  return true;
}

string nil::to_string() const {
  return "nil";
}


/* STREAMS */
// INPUT STREAM
void Istream::mark_node() {
  marked = true;
}

string Istream::to_string() const {
  return "input stream";
}

Istream::~Istream() {
  if (stream != &std::cin) {
    delete stream;
  }
}

// OUTPUT STREAM
void Ostream::mark_node() {
  marked = true;
}

string Ostream::to_string() const {
  return "output stream";
}

Ostream::~Ostream() {
  if (stream != &std::cout) {
    delete stream;
  }
}

// INPUT-OUTPUT STREAM
void IOstream::mark_node() {
  marked = true;
}

string IOstream::to_string() const {
  return "input/output stream";
}

IOstream::~IOstream() {
  delete stream;
}

void Ref::mark_node() {
  if (marked) return;
  marked = true;
  ptr->mark_node();
}

string Ref::to_string() const{
  return "(ref " + ptr->to_string() + ")";
}
