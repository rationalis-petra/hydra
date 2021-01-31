#include <string>

#include "expressions.hpp"
#include "operations.hpp"
#include "types.hpp"

hydra_type* type_from_rep(hydra_object* type) {
  if (hydra_symbol *sym = dynamic_cast<hydra_symbol *>(type)) {
    if (sym->name == "integer") {
      return new type_integer;
    }
    if (sym->name == "symbol") {
      return new type_symbol;
    }
    if (sym->name == "list") {
      return new type_list;
    }
    if (sym->name == "cons") {
      return new type_cons;
    }
    if (sym->name == "string") {
      return new type_string;
    }
    if (sym->name == "module") {
      return new type_module;
    }
  }
  return new type_integer();
}
