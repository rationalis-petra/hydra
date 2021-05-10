#ifndef __HYDRA_EXPRESSIONS_DATA_CTYPES_HPP
#define __HYDRA_EXPRESSIONS_DATA_CTYPES_HPP

#include <string>

#include "ffi.h"

#include "expressions/object.hpp"
#include "expressions/operation.hpp"


namespace expr {

struct CProxy;

// Used to represent a parent-class for a datatype, e.g. Integer,
// Char, etc.


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

struct CEnumType: public CType {
  CEnumType(std::unordered_map<Symbol*, int> enum_map);
  ffi_type* get_ffi_type();
  std::string to_string(interp::LocalRuntime &r, interp::LexicalScope& s);
  CProxy *get_from_object(Object* obj);
  CProxy *get_from_bits(unsigned char* arr, int idx, int len);

  std::unordered_map<Symbol*, int> enum_map;
};

struct CPtrType : public CType {
  CPtrType(CType* points_to);
  ffi_type* get_ffi_type();
  CProxy *get_from_object(Object* obj);
  CProxy *get_from_bits(unsigned char* arr, int idx, int len);
  std::string to_string(interp::LocalRuntime &r, interp::LexicalScope& s);

  CType* points_to;
};

struct CVoidType : public CType {
  CVoidType();
  ffi_type* get_ffi_type();
  std::string to_string(interp::LocalRuntime &r, interp::LexicalScope& s);

  CProxy *get_from_object(Object* obj);
  CProxy *get_from_bits(unsigned char* arr, int idx, int len);
};


struct CFnType : public CType {
  CFnType(CType* rettype, std::list<CType*> argtypes);

  ffi_type* get_ffi_type();
  CProxy *get_from_object(Object* obj);
  CProxy *get_from_bits(unsigned char* arr, int idx, int len);
  std::string to_string(interp::LocalRuntime &r, interp::LexicalScope& s);

  CType* return_type;
  std::list<CType*> argtypes;
};

} // namespace expr

#endif
