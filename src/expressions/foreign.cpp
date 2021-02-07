#include <string>
#include <stdio.h>

#include "ltdl.h"
#include "expressions.hpp"

using std::string;

// LIBRARIES
hydra_foreign_lib::hydra_foreign_lib(lt_dlhandle _lib) : lib(_lib){}

void hydra_foreign_lib::mark_node() {
  marked = true;
}
    
string hydra_foreign_lib::to_string() const {
  return "<foreign library>";
}




// SYMBOLS
void hydra_foreign_sym::mark_node() {
  marked = true;
}
hydra_foreign_sym::hydra_foreign_sym(void* addr) {
  address = addr;
}

string hydra_foreign_sym::to_string() const {
  char buffer[50];
  sprintf(buffer, "<foreign symbol: %p>", address);
  return string(buffer);
}


