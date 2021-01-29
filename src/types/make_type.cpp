#include <string>

#include "expressions.hpp"
#include "operations.hpp"
#include "types.hpp"

hydra_type* type_from_rep(hydra_object* type) {
  if (hydra_symbol *sym = dynamic_cast<hydra_symbol *>(type)) {
    if (sym->name == "integer") {
      return new type_integer();
    }
  }
  return new type_integer();
}
