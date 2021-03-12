#include <string>
#include <stdio.h>

#include "ltdl.h"
#include "expressions.hpp"

using std::string;
using namespace expr;

// LIBRARIES
ForeignLib::ForeignLib(lt_dlhandle _lib) : lib(_lib){}

void ForeignLib::mark_node() {
  marked = true;
}
    
string ForeignLib::to_string() const {
  return "<foreign library>";
}


// SYMBOLS
void ForeignSymbol::mark_node() {
  marked = true;
}

ForeignSymbol::ForeignSymbol(void* addr) {
  address = addr;
}

string ForeignSymbol::to_string() const {
  char buffer[50];
  sprintf(buffer, "<foreign symbol: %p>", address);
  return string(buffer);
}


