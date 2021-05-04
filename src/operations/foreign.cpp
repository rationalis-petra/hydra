#include <list>
#include <string>

#include "ffi.h"
#include "ltdl.h"

#include "operations.hpp"
#include "types.hpp"
#include "utils.hpp"

using std::list;
using std::string;

using namespace expr;
using namespace interp;

Object *op_foreign_lib(list<Object *> arg_list, LocalRuntime &r,
                       LexicalScope &s) {
  HString *str = get_inbuilt<HString *>(arg_list.back());

  lt_dlhandle lib = lt_dlopen(str->value.c_str());

  if (lib == NULL) {
    return nil::get();
  } else {
    return new ForeignLib(lib);
  }
}

Object *op_foreign_sym(list<Object *> arg_list, LocalRuntime &r,
                       LexicalScope &s) {
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
    return new UntypedProxy(addr);
  }
}

// define-foreign-function
Object *op_internalize(list<Object *> arg_list, LocalRuntime &r,
                       LexicalScope &s) {
  UntypedProxy *sym = get_inbuilt<UntypedProxy *>(arg_list.front());
  arg_list.pop_front();
  // TODO: more general CTypes...
  CFnType *type = get_inbuilt<CFnType *>(arg_list.front());

  // for now, we assume we're creating a function
  ForeignOperator *op = new ForeignOperator(type);
  op->fn_address = (void (*)(void))sym->address;

  ffi_type **arg_types = new ffi_type *[op->type->argtypes.size()];
  unsigned i = 0;
  for (CType* t : type->argtypes) {
    arg_types[i] = t->get_ffi_type();
    i++;
  }

  CType *returntype = type->return_type;

  ffi_type *rettype;
  rettype = returntype->get_ffi_type();

  // TODO: what is ffi_default_abi??
  // now call function
  if (ffi_prep_cif(&(op->fn_def), FFI_DEFAULT_ABI, type->argtypes.size(),
                   rettype, arg_types) == FFI_OK) {
    return op;
  } else {
    return nil::get();
  }
}

Operator *op::foreign_lib;
Operator *op::foreign_sym;
Operator *op::internalize;

void op::initialize_foreign() {
  int open_succ = lt_dlinit();
  if (!(open_succ == 0)) {
    string err = "Error initializing ltdl";
    throw err;
  }

  op::foreign_lib = new InbuiltOperator(
      "foreign-lib",
      "takes a string and tries to open the "
      "corresponding\n shared object (.so) or"
      "dynamic link library (.dll) file",
      op_foreign_lib, type::Fn::with_args({type::string_type}), true);

  op::foreign_sym = new InbuiltOperator(
      "foreign-sym",
      "Takes a string and a foreign library, and"
      "will find the\n foreign symbol whose name"
      "matches this string in the library",
      op_foreign_sym, type::Fn::with_args({new type::Any, type::string_type}),
      true);

  op::internalize = new InbuiltOperator(
      "internalize",
      "Takes a foreign symbol and a type declaration"
      "and returns a hydra\n value that can be used"
      "to access the symbol",
      op_internalize, type::Fn::with_args({new type::Any, type::c_type_type}),
      true);
};


