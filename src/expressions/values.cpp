#include <string>
#include <iostream>

#include "expressions.hpp"

using std::string;
using std::to_string;

using namespace expr;

// INPUT STREAM
void Istream::mark_node() {
  marked = true;
}

string Istream::to_string(LocalRuntime &r, LexicalScope &s) {
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

string Ostream::to_string(LocalRuntime &r, LexicalScope &s) {
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

string IOstream::to_string(LocalRuntime &r, LexicalScope &s) {
  return "input/output stream";
}

IOstream::~IOstream() {
  delete stream;
}
