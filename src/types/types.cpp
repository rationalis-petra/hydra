#include <string>

#include "expressions.hpp"
#include "types.hpp"

using std::string;

bool typep(hydra_type t, hydra_object* o) {
  switch(t.tag) {
    // simple types
  case hydra_type::type_int:
    return !(dynamic_cast<hydra_num*>(o) == nullptr);
  case hydra_type::type_char:
    return !(dynamic_cast<hydra_char*>(o) == nullptr);
  case hydra_type::type_cons:
    return !(dynamic_cast<hydra_cons*>(o) == nullptr);
  case hydra_type::type_symbol:
    return !(dynamic_cast<hydra_cons*>(o) == nullptr);
  case hydra_type::type_null:
    return !(dynamic_cast<hydra_cons*>(o) == nullptr);
  case hydra_type::type_t:
    return true;

    // foreign stuff
  case hydra_type::type_flib:
    return !(dynamic_cast<hydra_foreign_lib*>(o) == nullptr);
  case hydra_type::type_fsym:
    return !(dynamic_cast<hydra_foreign_sym*>(o) == nullptr);

    // composite types
  case hydra_type::type_fn: {
    hydra_oper* op = dynamic_cast<hydra_oper*>(o);
    if (op == nullptr) {
      return false;
    }
    for (hydra_type tp : t.type_params) {
    }
  }

  case hydra_type::type_list: {
    hydra_cons* cns;
    if (!(dynamic_cast<hydra_nil*>(o) == nullptr)) {
      return true;
    }
    hydra_type tp = t.type_params.front();
    if (!((cns = dynamic_cast<hydra_cons*>(o)) == nullptr)) {
      return false;
    }
    // for (hydra_object* c : cns) {
    //   if (!typep(tp, c)) {
    //     return false;
    //   }
    // }
    return true;
  }

  // case hydra_type::complex_list: {
  //   hydra_cons* cns;
  //   if (!(dynamic_cast<hydra_nil*>(o) == nullptr) && t.type_params.size() == 0) {
  //     return true;
  //   }
  //   if (!((cns = dynamic_cast<hydra_cons*>(o)) == nullptr)) {
  //     return false;
  //   }
  //   for (hydra_object* c : cns) {
  //     hydra_type tp = t.type_params.front();
  //     if (!typep(tp, c)) {
  //       return false; 
  //     }
  //   }
  //   return true;
  // }

  default:
    string err = "typep called with unsupported enum value!";
    throw err;
  }
}

void function_typecheck(hydra_type t, hydra_object* o, string fn_name) {
  if (!typep(t, o)) {
    if (fn_name == "") {
      fn_name = "Anonymous function";
    }
    string err = "Error calling " + fn_name + " provided args don't satisfy the type";
  }
}
