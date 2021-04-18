#include <string>
#include <iostream>

#include "expressions.hpp"
#include "utils.hpp"

using std::string;
//using std::to_string;

using namespace expr;
using namespace interp;

// INPUT STREAM
Parent* Istream::parent;
Istream::Istream() {
  Symbol* pt = get_keyword("parent");
  parents.insert(pt);
  slots[pt] = parent;
}

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
Parent* Ostream::parent;
Ostream::Ostream() {
  Symbol* pt = get_keyword("parent");
  parents.insert(pt);
  slots[pt] = parent;
}
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
Parent* IOstream::parent;
IOstream::IOstream() {
  Symbol* pt = get_keyword("parent");
  parents.insert(pt);
  slots[pt] = parent;
}
void IOstream::mark_node() {
  marked = true;
}

string IOstream::to_string(LocalRuntime &r, LexicalScope &s) {
  return "input/output stream";
}
IOstream::~IOstream() {
  delete stream;
}
