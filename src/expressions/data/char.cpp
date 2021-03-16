#include <string>
#include <typeinfo>

#include "expressions.hpp"

using namespace std;
using namespace expr;

Char::Char() : Object() {}

Char::Char(int c) : value(c) {}

void Char::mark_node() { marked = true; }

string Char::to_string() const { return string("") + ((char)value); }

Parent* Char::parent;
