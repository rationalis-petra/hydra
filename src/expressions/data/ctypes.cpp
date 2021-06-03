#include "utils.hpp"

#include "expressions/data/ctypes.hpp"
#include "expressions/data/cvals.hpp"
#include "expressions/symbol.hpp"
#include "expressions/data.hpp"
#include "types.hpp"

using namespace interp;
using namespace expr;

using std::list;
using std::pair;
using std::string;
using std::unordered_map;

// CBASIC_TYPE
Parent *CType::parent;
Parent *CType::modifier_parent;

CBasicType::CBasicType(BasicType _type) {
  Symbol *sym = get_keyword("parent");
  slots[sym] = parent;
  parents.insert(sym);

  type = _type;
  mod = sign_none;
  size = size_none;
}

ffi_type *CBasicType::get_ffi_type() {
  string err = "error in CBasicType::get_ffi_type";
  switch (mod) {
  case sign_none: {
    switch (size) {
    case size_none:
      switch (type) {
      case tchar:
        return &ffi_type_uchar;
      case tint:
        return &ffi_type_sint;
      case tfloat:
        return &ffi_type_float;
      case tdouble:
        return &ffi_type_double;
      }
      // short int is the only valid type
      // all others are errors...
    case size_short:
      switch (type) {
      case tint:
        return &ffi_type_sshort;
      default:
        throw err;
      }
      break;
    case size_long:
      switch (type) {
      case tint:
        return &ffi_type_slong;
      case tdouble:
        return &ffi_type_longdouble;
      default:
        throw err;
      }
    case size_long_long:
      throw err;
    }
  }
  case sign_no: {
    switch (size) {
    case size_none:
      switch (type) {
      case tchar:
        return &ffi_type_uchar;
      case tint:
        return &ffi_type_uint;
      default:
        throw err;
      }
      break;
      // short int is the only valid type
      // all others are errors...
    case size_short:
      switch (type) {
      case tint:
        return &ffi_type_ushort;
      default:
        throw err;
      }
      break;
    case size_long:
      switch (type) {
      case tint:
        return &ffi_type_ulong;
      default:
        throw err;
      }
    case size_long_long:
      throw err;
    }
  }
  case sign_yes: {
    switch (size) {
    case size_none:
      switch (type) {
      case tchar:
        return &ffi_type_schar;
      case tint:
        return &ffi_type_sint;
      default:
        throw err;
      }
      break;
      // short int is the only valid type
      // all others are errors...
    case size_short:
      switch (type) {
      case tint:
        return &ffi_type_sshort;
      default:
        throw err;
      }
      break;
    case size_long:
      switch (type) {
      case tint:
        return &ffi_type_slong;
      default:
        throw err;
      }
    case size_long_long:
      throw err;
    }
  }
  }
  throw err;
}

CProxy *CBasicType::get_from_bits(unsigned char *arr, int idx, int len) {
  string err = "error in CBasicType::get_from_bits";
  switch (mod) {
  case sign_none: {
    switch (size) {
    case size_none:
      switch (type) {
      case tchar:
        return new CBasic<unsigned char>(arr, idx, len);
      case tint:
        return new CBasic<signed int>(arr, idx, len);
      case tfloat:
        return new CBasic<float>(arr, idx, len);
      case tdouble:
        return new CBasic<double>(arr, idx, len);
      }
      // short int is the only valid type
      // all others are errors...
    case size_short:
      switch (type) {
      case tint:
        return new CBasic<signed short>(arr, idx, len);
      default:
        throw err;
      }
      break;
    case size_long:
      switch (type) {
      case tint:
        return new CBasic<signed long>(arr, idx, len);
      case tdouble:
        return new CBasic<long double>(arr, idx, len);
      default:
        throw err;
      }
    case size_long_long:
      throw err;
    }
  }
  case sign_no: {
    switch (size) {
    case size_none:
      switch (type) {
      case tchar:
        return new CBasic<unsigned char>(arr, idx, len);
      case tint:
        return new CBasic<unsigned int>(arr, idx, len);
      default:
        throw err;
      }
      break;
      // short int is the only valid type
      // all others are errors...
    case size_short:
      switch (type) {
      case tint:
        return new CBasic<unsigned short>(arr, idx, len);
      default:
        throw err;
      }
      break;
    case size_long:
      switch (type) {
      case tint:
        return new CBasic<unsigned long>(arr, idx, len);
      default:
        throw err;
      }
    case size_long_long:
      throw err;
    }
  }
  case sign_yes: {
    switch (size) {
    case size_none:
      switch (type) {
      case tchar:
        return new CBasic<signed char>(arr, idx, len);
      case tint:
        return new CBasic<signed int>(arr, idx, len);
      default:
        throw err;
      }
      break;
      // short int is the only valid type
      // all others are errors...
    case size_short:
      switch (type) {
      case tint:
        return new CBasic<signed short>(arr, idx, len);
      default:
        throw err;
      }
      break;
    case size_long:
      switch (type) {
      case tint:
        return new CBasic<signed long>(arr, idx, len);
      default:
        throw err;
      }
    case size_long_long:
      throw err;
    }
  }
  }
  throw err;
}

CProxy *CBasicType::get_from_object(Object *obj) {
  if (CProxy* cpx = get_inbuilt<CProxy*>(obj)) {
    if (cpx->get_type()) {
    }
  }
  switch (mod) {
  case sign_none: {
    switch (size) {
    case size_none:
      switch (type) {
      case tchar:
        if (Char *chr = get_inbuilt<Char *>(obj)) {
          return new CBasic<char>(chr->value);
        }
        break;
      case tint:
        if (Integer *inte = get_inbuilt<Integer *>(obj)) {
          return new CBasic<int>(inte->get_sl());
        }
        break;
      case tfloat:
        if (Float *flot = get_inbuilt<Float *>(obj)) {
          return new CBasic<float>(flot->value);
        }
        break;
      case tdouble:
        if (Float *flot = get_inbuilt<Float *>(obj)) {
          return new CBasic<double>(flot->value);
        }
        break;
      }
      // short int is the only valid type
      // all others are errors...
    case size_short:
      switch (type) {
      case tint:
        if (Integer *sho = get_inbuilt<Integer *>(obj)) {
          return new CBasic<short>(sho->get_sl());
        }
        break;
      default:
        break;
      }
      break;
    case size_long:
      switch (type) {
      case tint:
        if (Integer *lng = get_inbuilt<Integer *>(obj)) {
          return new CBasic<long>(lng->get_sl());
        }
        break;
      case tdouble:
        if (Float *flot = get_inbuilt<Float *>(obj)) {
          return new CBasic<float>(flot->value);
        }
        break;
      default:
        break;
      }
    case size_long_long:
      break;
    }
  }
  case sign_no: {
    switch (size) {
    case size_none:
      switch (type) {
      case tchar:
        if (Char *chr = get_inbuilt<Char *>(obj)) {
          return new CBasic<unsigned char>(chr->value);
        }
        break;
      case tint:
        if (Integer *inte = get_inbuilt<Integer *>(obj)) {
          return new CBasic<unsigned int>(inte->get_ul());
        }
        break;
      default:
        break;
      }
      break;
      // short int is the only valid type
      // all others are errors...
    case size_short:
      switch (type) {
      case tint:
        if (Integer *inte = get_inbuilt<Integer *>(obj)) {
          return new CBasic<unsigned short>(inte->get_ul());
        }
        break;
      default:
        break;
      }
      break;
    case size_long:
      switch (type) {
      case tint:
        if (Integer *inte = get_inbuilt<Integer *>(obj)) {
          return new CBasic<unsigned long>(inte->get_ul());
        }
        break;
      default:
        break;
      }
    case size_long_long:
      break;
    }
  }
  case sign_yes: {
    switch (size) {
    case size_none:
      switch (type) {
      case tchar:
        if (Char *chr = get_inbuilt<Char *>(obj)) {
          return new CBasic<signed char>(chr->value);
        }
        break;
      case tint:
        if (Integer *inte = get_inbuilt<Integer *>(obj)) {
          return new CBasic<signed int>(inte->get_sl());
        }
        break;
      default:
        break;
      }
      break;
      // short int is the only valid type
      // all others are errors...
    case size_short:
      switch (type) {
      case tint:
        if (Integer *inte = get_inbuilt<Integer *>(obj)) {
          return new CBasic<signed short>(inte->get_sl());
        }
        break;
      default:
        break;
      }
      break;
    case size_long:
      switch (type) {
      case tint:
        if (Integer *inte = get_inbuilt<Integer *>(obj)) {
          return new CBasic<signed long>(inte->get_sl());
        }
        break;
      default:
        break;
      }
    case size_long_long:
      break;
    }
  }
  }
  string err = "error in CBasicType::get_from_object: no applicable conversion";
  throw err;
}

string CBasicType::to_string(LocalRuntime &r, LexicalScope &s) {
  switch (mod) {
  case sign_none: {
    switch (size) {
    case size_none:
      switch (type) {
      case tchar:
        return "char";
      case tint:
        return "int";
      case tfloat:
        return "float";
      case tdouble:
        return "double";
      }
      // short int is the only valid type
      // all others are errors...
    case size_short:
      switch (type) {
      case tint:
        return "short";
      default:
        break;
      }
      break;
    case size_long:
      switch (type) {
      case tint:
        return "long";
      case tdouble:
        return "long double";
      default:
        break;
      }
    case size_long_long:
      break;
    }
  }
  case sign_no: {
    switch (size) {
    case size_none:
      switch (type) {
      case tchar:
        return "unsigned char";
      case tint:
        return "unsigned int";
      default:
        break;
      }
      break;
      // short int is the only valid type
      // all others are errors...
    case size_short:
      switch (type) {
      case tint:
        return "unsigned short int";
      default:
        break;
      }
      break;
    case size_long:
      switch (type) {
      case tint:
        return "unsigned long int";
      default:
        break;
      }
    case size_long_long:
      return "unsigned long long";
    }
  }
  case sign_yes: {
    switch (size) {
    case size_none:
      switch (type) {
      case tchar:
        return "signed char";
      case tint:
        return "signed int";
      default:
        break;
      }
      break;
      // short int is the only valid type
      // all others are errors...
    case size_short:
      switch (type) {
      case tint:
        return "signed short int";
      default:
        break;
      }
      break;
    case size_long:
      switch (type) {
      case tint:
        return "signed long int";
      default:
        break;
      }
    case size_long_long:
      return "signed long long";
    }
  }
  }
  return "bad type combination";
};

// CPOINTER TYPE

CPtrType::CPtrType(CType *_points_to) : points_to(_points_to) {
  Symbol *sym = get_keyword("parent");
  slots[sym] = parent;
  parents.insert(sym);
}

CProxy *CPtrType::get_from_object(Object* obj) {
  // only works for CProxy
  if (CProxy *pxy = get_inbuilt<CProxy*>(obj)) {
    return new CPtr(pxy->get_type(), pxy->get_ref());
  }
  // special case: character arrays!
  if (CBasicType* bsc = dynamic_cast<CBasicType*>(points_to)) {
    if (bsc->type == tchar) {
      if (HString* str = get_inbuilt<HString*>(obj)) {
        return new CPtr(bsc, (void*) str->value.c_str());
      }
    }
  }
  
  string err = "Can't get pointer for object";
  throw err;
}

CProxy *CPtrType::get_from_bits(unsigned char* arr, int idx, int len) {
  union {
    unsigned char arr[sizeof(void*)];
    void* val;
  } ptr;
  for (size_t i = 0; i < sizeof(void*); i++) {
    if (i+idx > len) {
      string err = "bit overrun in CPtrType::get_from_bits";
      throw err;
    }
    ptr.arr[i] = arr[i+idx];
  }
  return new CPtr(new CVoidType, ptr.val);
}

ffi_type *CPtrType::get_ffi_type() { return &ffi_type_pointer; }

string CPtrType::to_string(LocalRuntime &r, LexicalScope &s) {
  return "<CPointer Type>";
}













// CSTRUCT TYPE

CStructType::CStructType(list<pair<Symbol *, CType *>> types) {

  Symbol *sym = get_keyword("parent");
  slots[sym] = parent;
  parents.insert(sym);

  int i = 0;
  ffi_type **type_elements = new ffi_type *[types.size() + 1];
  for (auto p : types) {
    type_elements[i++] = p.second->get_ffi_type();
  }
  type_elements[types.size()] = nullptr;
  struct_type = {.size = 0,
                 .alignment = 0,
                 .type = FFI_TYPE_STRUCT,
                 .elements = type_elements};
}

ffi_type *CStructType::get_ffi_type() { return &struct_type; }

string CStructType::to_string(LocalRuntime &r, LexicalScope &s) {
  string out = "[";
  // TODO: order by indices
  for (pair p : types) {
    out += p.first->to_string(r, s);
    out += p.second->to_string(r, s);
    out += '\n';
  }
  out += "]";
  return out;
};

struct CStructType : public CType {
  CStructType();
  ffi_type *get_ffi_type();
  std::string to_string(interp::LocalRuntime &r, interp::LexicalScope &s);
  ffi_type struct_type;

  // a data structure enabling the retrieval of data from
  std::map<Symbol *, CType *> type_names;
  std::vector<CType *> types;
};

CModifierSize::CModifierSize(string _name, string _docstring,
                             Object *(*_fnc)(std::list<Object *> arg_list,
                                             LocalRuntime &r, LexicalScope &s),
                             SizeModifier size)
    : InbuiltOperator(_name, _docstring, _fnc,
                      type::Fn::with_rest(type::c_type_type), true) {

  Symbol *sym = get_keyword("parent");
  slots[sym] = CType::modifier_parent;
  parents.insert(sym);
  size_mod = size;
}

CModifierSign::CModifierSign(string _name, string _docstring,
                             Object *(*_fnc)(std::list<Object *> arg_list,
                                             LocalRuntime &r, LexicalScope &s),
                             SignModifier sign)
    : InbuiltOperator(_name, _docstring, _fnc,
                      type::Fn::with_rest(type::c_type_type), true) {

  Symbol *sym = get_keyword("parent");
  slots[sym] = CType::modifier_parent;
  parents.insert(sym);
  sign_mod = sign;
}

CVoidType::CVoidType() {
  Symbol *sym = get_keyword("parent");
  slots[sym] = CType::modifier_parent;
  parents.insert(sym);
}

CProxy *CVoidType::get_from_object(Object *obj) {
  string err = "cannot get_from_object on cvoidtype";
  throw err;
}
CProxy *CVoidType::get_from_bits(unsigned char *arr, int idx, int len) {
  string err = "cannot get_from_bits on cvoidtype";
  throw err;
}

ffi_type* CVoidType::get_ffi_type() {
  return &ffi_type_void;
}

std::string CVoidType::to_string(interp::LocalRuntime &r, interp::LexicalScope& s) {
  return "<CType: void>";
}


CFnType::CFnType(CType *rettype, std::list<CType *> _argtypes)
    : return_type(rettype), argtypes(_argtypes) {
  Symbol *sym = get_keyword("parent");
  slots[sym] = CType::modifier_parent;
  parents.insert(sym);
}

ffi_type* CFnType::get_ffi_type() {
  string err = "TODO: implement get_ffi_type for functions...";
  throw err;
}

CProxy *CFnType::get_from_object(Object *obj) {
  string err = "TODO: implement get_from_object for functions...";
  throw err;
}

CProxy *CFnType::get_from_bits(unsigned char *arr, int idx, int len) {
  string err = "TODO: implement get_from_bits for functions...";
  throw err;
}

std::string CFnType::to_string(interp::LocalRuntime &r,
                               interp::LexicalScope &s) {
  return "<CFnType>";
}

CEnumType::CEnumType(unordered_map<Symbol *, int> map) : enum_map(map) {
  Symbol *sym = get_keyword("parent");
  slots[sym] = parent;
  parents.insert(sym);
}

ffi_type* CEnumType::get_ffi_type() {
  return &ffi_type_sint;
}
std::string CEnumType::to_string(interp::LocalRuntime &r, interp::LexicalScope& s) {
  return "C enum";
}

CProxy *CEnumType::get_from_object(Object* obj) {
  if (Symbol* sym = get_inbuilt<Symbol*>(obj)) {
    return new CBasic<int>(enum_map[sym]);
  } else {
    string err =  "C enums require symbols";
    throw err;
  }
}

CProxy *CEnumType::get_from_bits(unsigned char* arr, int idx, int len) {
  CBasicType t(expr::tint);
  return t.get_from_bits(arr, idx, len);
}

