#include <string>
#include <stdio.h>

#include "expressions.hpp"

using std::string;

hydra_foreign_lib::hydra_foreign_lib(lt_dlhandle _lib) : lib(_lib){}

string hydra_foreign_lib::to_string() const {
  return "<foreign library>";
}

hydra_foreign_sym::hydra_foreign_sym(void* addr) {
  address = addr;
}

string hydra_foreign_sym::to_string() const {
  char buffer[50];
  sprintf(buffer, "<foreign symbol: %p>", address);
  return string(buffer);
}


