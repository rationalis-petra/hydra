#include <string>
#include <list>

#include "ffi.h"
#include "ltdl.h"

#include "operations.hpp"

using std::list;
using std::string;
using std::to_string;

op_foreign_lib::op_foreign_lib() { is_fn = true; }
hydra_object *op_foreign_lib::call(hydra_object *body, runtime &r, lexical_scope &s) {
  list<hydra_object *> arg_list = get_arg_list(body, r, s);
  if (arg_list.size() != 1) {
    string err =
        "Incorrect number of arguments provided to function load-foreign-lib";
    throw err;
  }
  hydra_string *str = dynamic_cast<hydra_string *>(arg_list.back());
  if (!str) {
    string err = "load-foreign-lib requires a string second argument";
    throw err;
  }
  int open_succ = lt_dlinit();
  if (!(open_succ == 0)) {
    string err = "Error initializing ltdl";
    throw err;
  }
  lt_dlhandle lib = lt_dlopen(str->value.c_str());

  if (lib == NULL) {
    return new hydra_nil;
  } else {
    return new hydra_foreign_lib(lib);
  }
}

op_foreign_sym::op_foreign_sym() { is_fn = true; }
hydra_object *op_foreign_sym::call(hydra_object *body, runtime &r, lexical_scope &s) {
  list<hydra_object *> arg_list = get_arg_list(body, r, s);
  if (arg_list.size() != 2) {
    string err =
        "Incorrect number of arguments provided to function get-foreign-symbol";
    throw err;
  }
  hydra_foreign_lib *lib = dynamic_cast<hydra_foreign_lib *>(arg_list.front());
  hydra_string *str = dynamic_cast<hydra_string *>(arg_list.back());
  if (!lib) {
    string err =
        "First argument to get-foreign-symbol must be of type foreign-library";
    throw err;
  }
  if (!str) {
    string err = "Second argument to get-foreign-symbol must be of type string";
    throw err;
  }

  void *addr = lt_dlsym(lib->lib, str->value.c_str());
  if (addr == nullptr) {
    return new hydra_nil;
  } else {
    return new hydra_foreign_sym(addr);
  }
}
// define-foreign-function
op_internalize::op_internalize() { is_fn = true; }
hydra_object *op_internalize::call(hydra_object *body, runtime &r, lexical_scope &s) {
  list<hydra_object *> arg_list = get_arg_list(body, r, s);
  if (arg_list.size() != 2) {
    string err =
        "Expected 2 arguments when internalizing foreign symbol, got + " +
        ::to_string(arg_list.size());
    throw err;
  }
  hydra_foreign_sym *sym = dynamic_cast<hydra_foreign_sym *>(arg_list.front());
  hydra_cons *type = dynamic_cast<hydra_cons *>(arg_list.back());
  if (sym == nullptr) {
    string err =
        "Expected first argument to internalize to be a foreign symbol";
    throw err;
  }
  if (type == nullptr) {
    string err = "yo, idiot, fix the type-parsing to internalize!";
    throw err;
  }


  // for now, we assume we're creating a function
  hydra_foreign_oper *op = new hydra_foreign_oper();
  op->fn_address = (void (*)(void))sym->address;
  hydra_object *arglst = dynamic_cast<hydra_cons *>(type->cdr)->car;
  while (!arglst->null()) {
    // type-check here
    hydra_cons *cell = dynamic_cast<hydra_cons *>(arglst);
    hydra_symbol *sym = dynamic_cast<hydra_symbol *>(cell->car);
    if (sym->name == "void") {
      op->arg_types.push_back(Void);
    } else if (sym->name == "int-32") {
      op->arg_types.push_back(Int32);
    } else if (sym->name == "pointer") {
      op->arg_types.push_back(Pointer);
    } else if (sym->name == "string") {
      op->arg_types.push_back(String);
    } else {
      string err = "type of foreign argument not recognized";
      throw err;
    }
    arglst = cell->cdr;
  }



  // arg_list[0] = foreign symbol
  // create a foreign function given a symbol and a type list
  // caller interface structure
  // array of types
  ffi_type **arg_types = new ffi_type *[op->arg_types.size()];
  unsigned i = 0;
  for (foreign_type t : op->arg_types) {
    switch (t) {
    case Int32:
      arg_types[i] = &ffi_type_sint;
      break;
    case Pointer:
      arg_types[i] = &ffi_type_pointer;
      break;
    case String:
      arg_types[i] = &ffi_type_pointer;
      break;
    case Void:
      arg_types[i] = &ffi_type_void;
      break;
    }
    i++;
  }

  hydra_object *return_type =
      dynamic_cast<hydra_cons *>(dynamic_cast<hydra_cons *>(type->cdr)->cdr)
          ->car;
  hydra_symbol *return_sym = dynamic_cast<hydra_symbol *>(return_type);
  if (return_sym == nullptr) {
    string err = "no return type provided!";
    throw err;
  }

  ffi_type *rettype;
  if (return_sym->name == "void") {
    op->return_type = Void;
    rettype = &ffi_type_void;
  } else if (return_sym->name == "int-32") {
    op->return_type = Int32;
    rettype = &ffi_type_sint;
  } else if (return_sym->name == "pointer") {
    op->return_type = Pointer;
    rettype = &ffi_type_pointer;
  } else if (return_sym->name == "string") {
    op->return_type = String;
    rettype = &ffi_type_pointer;
  } else {
    string err = "type of return symbol not recognized";
    throw err;
  }

  // TODO: what is ffi_default_abi??
  // now call function
  if (ffi_prep_cif(&(op->fn_def), FFI_DEFAULT_ABI, op->arg_types.size(), rettype,
                   arg_types) == FFI_OK) {
    return op;
  } else {
    return new hydra_nil;
  }
}



hydra_foreign_oper::hydra_foreign_oper() { is_fn = true; }
hydra_object *hydra_foreign_oper::call(hydra_object *alist, runtime &r, lexical_scope &s) {
  list<hydra_object *> arg_list = get_arg_list(alist, r, s);
  if (arg_list.size() != arg_types.size()) {
    string err =
        "Error in foreign function call: arg list is not of expected size";
    throw err;
  }

  void *mynull = nullptr;
  void **arg_values = new void *[arg_list.size()];

  list<hydra_object *>::iterator vals = arg_list.begin();
  list<foreign_type>::iterator types = arg_types.begin();
  for (unsigned i = 0; i < arg_list.size(); i++) {
    hydra_object *val = *vals;
    foreign_type type = *types;

    switch (type) {
    case Int32: {
      hydra_num *num = dynamic_cast<hydra_num *>(val);
      if (num == nullptr) {
        string err = "Provided non-num type to function!";
        throw err;
      }
      arg_values[i] = &num->value;
    } break;
    case Pointer:
      if (val->null()) {
        arg_values[i] = &mynull;
      } else if (hydra_foreign_sym* ptr = dynamic_cast<hydra_foreign_sym*>(val)) {
        arg_values[i] = &ptr->address;
      } else {
        string err = "Haven't properly implemented pointers yet...";
        throw err;
      }
      break;
    case String: {
      hydra_string *str = dynamic_cast<hydra_string *>(val);
      if (str == nullptr) {
        string err = "Provided non-num type to function!";
        throw err;
      } else {
        const char* cstr = str->value.c_str();
        arg_values[i] = &cstr;
      }
    } break;
    case Void:
      string err = "Void in arg_list?";
      throw err;
      break;
    }

    vals++;
    types++;
  }

  ffi_arg result;

  ffi_call(&fn_def, fn_address, &result, arg_values);
  switch (return_type) {
  case Void:
    return new hydra_nil;
    break;
  case Int32:
    return new hydra_num((int) result);
    break;
  case String: {
    const char *str = (const char *)result;
    hydra_string *out = new hydra_string();
    out->value = str;
    return out;
  } break;
  case Pointer: {
    return new hydra_foreign_sym((void*) result);
  } break;
  }
  string err = "reached end of foreign call??";
  throw err;
}
