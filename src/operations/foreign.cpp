#include <string>
#include <list>

#include "ffi.h"
#include "ltdl.h"

#include "operations.hpp"
#include "types.hpp"

using std::list;
using std::string;
using std::to_string;

using namespace expr;

Value *op_foreign_lib(Operator* op, Value *body, LocalRuntime &r,
                                   LexicalScope &s) {
  list<Value *> arg_list = op->get_arg_list(body, r, s);
  if (arg_list.size() != 1) {
    string err =
        "Incorrect number of arguments provided to function load-foreign-lib";
    throw err;
  }
  HString *str = dynamic_cast<HString *>(arg_list.back());
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
    return nil::get();
  } else {
    return new ForeignLib(lib);
  }
}

Operator *op::foreign_lib =
    new InbuiltOperator("takes a string and tries to open the "
                        "corresponding\n shared object (.so) or"
                        "dynamic link library (.dll) file",
                        op_foreign_lib,
                        type::Fn::with_args({new type::TString}),
                        true);

Value *op_foreign_sym(Operator* op, Value *body, LocalRuntime &r, LexicalScope &s) {
  list<Value *> arg_list = op->get_arg_list(body, r, s);
  if (arg_list.size() != 2) {
    string err =
        "Incorrect number of arguments provided to function get-foreign-symbol";
    throw err;
  }
  ForeignLib *lib = dynamic_cast<ForeignLib *>(arg_list.front());
  HString *str = dynamic_cast<HString *>(arg_list.back());
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
    return nil::get();
  } else {
    return new ForeignSymbol(addr);
  }
}


Operator* op::foreign_sym =
  new InbuiltOperator("Takes a string and a foreign library, and"
                      "will find the\n foreign symbol whose name"
                      "matches this string in the library",
                      op_foreign_sym,
                      type::Fn::with_args({new type::TString, new type::Nil}),
                      true);

// define-foreign-function
Value *op_internalize(Operator* op2, Value *body, LocalRuntime &r, LexicalScope &s) {
  list<Value *> arg_list = op2->get_arg_list(body, r, s);
  if (arg_list.size() != 2) {
    string err =
        "Expected 2 arguments when internalizing foreign symbol, got + " +
        ::to_string(arg_list.size());
    throw err;
  }
  ForeignSymbol *sym = dynamic_cast<ForeignSymbol *>(arg_list.front());
  Cons *type = dynamic_cast<Cons *>(arg_list.back());
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
  ForeignOperator *op = new ForeignOperator();

  op->fn_address = (void (*)(void))sym->address;
  Value *arglst = dynamic_cast<Cons *>(type->cdr)->car;
  while (!arglst->null()) {
    // type-check here
    Cons *cell = dynamic_cast<Cons *>(arglst);
    Symbol *sym = dynamic_cast<Symbol *>(cell->car);
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

  Value *return_type =
      dynamic_cast<Cons *>(dynamic_cast<Cons *>(type->cdr)->cdr)
          ->car;
  Symbol *return_sym = dynamic_cast<Symbol *>(return_type);
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
    return nil::get();
  }
}

Operator *op::internalize =
    new InbuiltOperator("Takes a foreign symbol and a type declaration"
                        "and returns a hydra\n value that can be used"
                        "to access the symbol",
                        op_internalize,
                        type::Fn::with_args({new type::Nil, new type::Nil}),
                        true);

Value *ForeignOperator::call(Value *alist, LocalRuntime &r, LexicalScope &s) {
  list<Value *> arg_list = get_arg_list(alist, r, s);
  if (arg_list.size() != arg_types.size()) {
    string err =
        "Error in foreign function call: arg list is not of expected size";
    throw err;
  }

  void *mynull = nullptr;
  void **arg_values = new void *[arg_list.size()];

  list<Value *>::iterator vals = arg_list.begin();
  list<foreign_type>::iterator types = arg_types.begin();
  for (unsigned i = 0; i < arg_list.size(); i++) {
    Value *val = *vals;
    foreign_type type = *types;

    switch (type) {
    case Int32: {
      Integer *num = dynamic_cast<Integer *>(val);
      if (num == nullptr) {
        string err = "Provided non-num type to function!";
        throw err;
      }
      arg_values[i] = &num->value;
    } break;
    case Pointer:
      if (val->null()) {
        arg_values[i] = &mynull;
      } else if (ForeignSymbol* ptr = dynamic_cast<ForeignSymbol*>(val)) {
        arg_values[i] = &ptr->address;
      } else {
        string err = "Haven't properly implemented pointers yet...";
        throw err;
      }
      break;
    case String: {
      HString *str = dynamic_cast<HString *>(val);
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
    return nil::get();
    break;
  case Int32:
    return new Integer((int) result);
    break;
  case String: {
    const char *str = (const char *)result;
    HString *out = new HString();
    out->value = str;
    return out;
  } break;
  case Pointer: {
    return new ForeignSymbol((void*) result);
  } break;
  }
  string err = "reached end of foreign call??";
  throw err;
}

ForeignOperator::ForeignOperator() {
  is_fn = true;
  docstring = new HString("A foriegn operator");
}
