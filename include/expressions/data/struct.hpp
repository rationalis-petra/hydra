#ifndef __HYDRA_EXPRESSIONS_DATA_CPROXIES_HPP
#define __HYDRA_EXPRESSIONS_DATA_CPROXIES_HPP

#include <string>

#include "ffi.h"

#include "expressions/object.hpp"
#include "expressions/operation.hpp"


namespace expr {

struct CProxy;

// Used to represent a parent-class for a datatype, e.g. Integer,
// Char, etc.

enum CModifierType { none, cmptr, cmshort, cmlong, cmsigned, cmunsigned };

struct CType : public Object {
  static Parent* parent;
  static Parent* modifier_parent;
  // convert a hydra object (Integer, Float, ...) into a C Proxy object
  virtual CProxy *get_from_object(Object* obj) = 0;
  virtual CProxy *get_from_bits(unsigned char* arr, int idx, int len) = 0;
  virtual ffi_type* get_ffi_type() = 0;
  unsigned char *bytes;
};

enum SignModifier { sign_none, sign_yes, sign_no };
struct CModifierSign : public InbuiltOperator {
  CModifierSign(std::string _name, std::string _docstring,
                Object *(*_fnc)(std::list<Object *> arg_list, interp::LocalRuntime &r,
                                interp::LexicalScope &s),
                SignModifier s);
  SignModifier sign_mod;
};

enum SizeModifier { size_none, size_long, size_long_long, size_short };
struct CModifierSize : public InbuiltOperator {
  CModifierSize(std::string _name, std::string _docstring,
                Object *(*_fnc)(std::list<Object *> arg_list, interp::LocalRuntime &r,
                                interp::LexicalScope &s),
                SizeModifier s);
  SizeModifier size_mod;
};


enum BasicType { tchar, tint, tfloat, tdouble };
struct CBasicType : CType {
  CBasicType(BasicType);
  std::string to_string(interp::LocalRuntime& r, interp::LexicalScope &s);
  CProxy *get_from_object(Object* obj);
  CProxy *get_from_bits(unsigned char* arr, int idx, int len);
  ffi_type *get_ffi_type();
  BasicType type;
  SizeModifier size;
  SignModifier mod;
};

struct CStructType : public CType {
  CStructType(std::list<std::pair<Symbol*, CType*>> types);
  ffi_type* get_ffi_type();
  ffi_type struct_type;
  std::string to_string(interp::LocalRuntime &r, interp::LexicalScope& s);

  // a data structure enabling the retrieval of data from  
  std::map<Symbol*, CType*> types;
  std::map<Symbol*, int> type_indices;
};

struct CPtrType : public CType {
  CPtrType(CType* points_to);
  ffi_type* get_ffi_type();
  std::string to_string(interp::LocalRuntime &r, interp::LexicalScope& s);

  CType* points_to;
};

// ACTUAL OBJECTS
struct CProxy : Object {
  CType* get_type();
};

template <typename T> struct CBasic : CProxy {
  CBasic(T val) {
    u.value = val;
  }

  CBasic(unsigned char* arr, int idx, int len) {
    if (idx + sizeof(T) > len) {
      std::string err = "attempt to construct CPoxy from bit-array would go "
                        "off end of bit-array";
      throw err;
    }

    for (unsigned i = 0; i < sizeof(T); i++) {
      u.data[i] = arr[idx + i];
    }
  };

  union {
    T value;
    unsigned char data [sizeof(T)];
  } u;
};


// Used to represent a parent-class for a datatype, e.g. Integer,
// Char, etc.
struct CStruct : public CProxy {
  CStruct();
  CStructType* type;
  std::string to_string(interp::LocalRuntime &r, interp::LexicalScope& s);

  unsigned char* data;
  int len;

  // a data structure enabling the retrieval of data from  
  CProxy *get_element(Symbol* name);
  CProxy *get_from_bits(unsigned char* data, int idx, int len);
};

  // Represetns a C pointer for FFI purposes
struct CPtr : public CProxy {
  CType* points_to;
  void *ptr;
};
} // namespace expr

#endif
