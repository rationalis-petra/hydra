#include <string>
#include <stdio.h>

#include "ltdl.h"
#include "expressions.hpp"

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


ForeignOperator::ForeignOperator() {
  is_fn = true;
  docstring = new HString("A foriegn operator");
  invoker = this;
}

Object *ForeignOperator::call(list<Object*> arg_list, LocalRuntime &r, LexicalScope &s, bool) {
  if (arg_list.size() != arg_types.size()) {
    string err =
        "Error in foreign function call: arg list is not of expected size";
    throw err;
  }

  void *mynull = nullptr;
  void **arg_values = new void *[arg_list.size()];

  list<Object *>::iterator vals = arg_list.begin();
  list<foreign_type>::iterator types = arg_types.begin();
  for (unsigned i = 0; i < arg_list.size(); i++) {
    Object *val = *vals;
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

