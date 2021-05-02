#include <string>
#include <stdio.h>

#include "ltdl.h"
#include "expressions.hpp"
#include "utils.hpp"

using std::string;
using std::list;
using namespace expr;
using namespace interp;

// LIBRARIES
ForeignLib::ForeignLib(lt_dlhandle _lib) : lib(_lib){}

void ForeignLib::mark_node() {
  Object::mark_node();
}
    
string ForeignLib::to_string(LocalRuntime &r, LexicalScope &s) {
  return "<foreign library>";
}


// SYMBOLS
void ForeignSymbol::mark_node() {
  marked = true;
}

ForeignSymbol::ForeignSymbol(void* addr) {
  address = addr;
}

string ForeignSymbol::to_string(LocalRuntime &r, LexicalScope &s) {
  char buffer[50];
  sprintf(buffer, "<foreign symbol: %p>", address);
  return string(buffer);
}


ForeignOperator::ForeignOperator(CFnType* _type) {
  type = _type;
  for (auto a : _type->argtypes) {
    Operator::type->arg_list.push_back(new type::Any);
  }
  is_fn = true;
  Symbol* dsym = get_keyword("docstring");
  metadata[dsym] = new HString("A foreign operator");
  invoker = this;
}

Object *ForeignOperator::call(list<Object *> arg_list, LocalRuntime &r,
                              LexicalScope &s, bool) {
  if (arg_list.size() != type->argtypes.size()) {
    string err =
        "Error in foreign function call: arg list is not of expected size";
    throw err;
  }

  void **arg_values = new void *[arg_list.size()];

  list<Object *>::iterator vals = arg_list.begin();
  list<CType*>::iterator types = type->argtypes.begin();
  for (unsigned i = 0; i < arg_list.size(); i++) {
    Object *val = *vals;
    CType* type = *types;

    CProxy *pxy = type->get_from_object(val);
    if (pxy) {
      arg_values[i] = pxy->get_ref();
    } else {
      string err = "could not construct CProxy for object...";
      throw err;
    }

    vals++;
    types++;
  }

  ffi_arg result;

  ffi_call(&fn_def, fn_address, &result, arg_values);

  if (dynamic_cast<CVoidType*>(type->return_type)) {
    return nil::get();
  } else {
    CProxy *out = type->return_type->get_from_bits((unsigned char *)result, 0,
                                                   sizeof(result));
    return out;
  }
}
