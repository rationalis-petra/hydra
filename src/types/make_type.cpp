#include <string>

#include "expressions.hpp"
#include "operations.hpp"
#include "types.hpp"

using std::string;

hydra_type* type_from_rep(hydra_object* type) {
  if (hydra_symbol *sym = dynamic_cast<hydra_symbol *>(type)) {
    if (sym->name == "Integer") {
      return new type_integer;
    }
    if (sym->name == "Symbol") {
      return new type_symbol;
    }
    if (sym->name == "List") {
      return new type_list;
    }
    if (sym->name == "Cons") {
      return new type_cons;
    }
    if (sym->name == "String") {
      return new type_string;
    }
    if (sym->name == "Module") {
      return new type_module;
    }
    if (sym->name == "Char") {
      return new type_char;
    }
    if (sym->name == "Stream") {
      return new type_stream;
    } 
    if (sym->name == "Type") {
      return new type_type;
    } 
    if (sym->name == "Fn") {
      return new type_fn;
    } 
  }
  string err = "bad thing to be a type specifier: " + type->to_string();
  throw err;
}
