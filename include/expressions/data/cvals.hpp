#ifndef __HYDRA_EXPRESSIONS_DATA_CVALS_HPP
#define __HYDRA_EXPRESSIONS_DATA_CVALS_HPP

#include <string>

#include "ffi.h"

#include "expressions/object.hpp"
#include "expressions/operation.hpp"


namespace expr {

struct CType;
struct CStructType;

struct CProxy : Object {
  virtual CType* get_type() = 0;
  virtual void* get_ref() = 0;
};

template <typename T> struct CBasic : CProxy {
  CBasic(T val) {
    u.value = val;
  }

  CBasic(unsigned char* arr, size_t idx, size_t len) {
    if (idx + sizeof(T) > len) {
      std::string err = "attempt to construct CPoxy from bit-array would go "
                        "off end of bit-array";
      throw err;
    }

    for (size_t i = 0; i < sizeof(T); i++) {
      u.data[i] = arr[idx + i];
    }
  };

  union {
    T value;
    unsigned char data [sizeof(T)];
  } u;

  void* get_ref() {
    return &u;
  }

  CType *get_type() { return nullptr; }

  std::string to_string(interp::LocalRuntime &r,
                        interp::LexicalScope &s) {
    return "<Proxy: " + std::to_string(u.value) + ">";
  }
};

template <> CType *CBasic<char>::get_type();
template <> CType *CBasic<short>::get_type();
template <> CType *CBasic<int>::get_type();
template <> CType *CBasic<long>::get_type();
template <> CType *CBasic<long long>::get_type();

template <> CType *CBasic<signed char>::get_type();

template <> CType *CBasic<unsigned char>::get_type();
template <> CType *CBasic<unsigned short>::get_type();
template <> CType *CBasic<unsigned int>::get_type();
template <> CType *CBasic<unsigned long>::get_type();
template <> CType *CBasic<unsigned long long>::get_type();

template <> CType* CBasic<float>::get_type();
template <> CType* CBasic<double>::get_type();
template <> CType* CBasic<long double>::get_type();


// Used to represent a parent-class for a datatype, e.g. Integer,
// Char, etc.
struct CStruct : public CProxy {
  CStruct();
  CType* get_type();
  void* get_ref();

  CStructType* type;
  std::string to_string(interp::LocalRuntime &r, interp::LexicalScope& s);

  unsigned char* data;
  int len;

  // a data structure enabling the retrieval of data from  
  CProxy *get_element(Symbol* name);
};

  // Represetns a C pointer for FFI purposes
struct CPtr : public CProxy {
  CPtr(CType* points_to, void* ptr);
  CType* get_type();
  void* get_ref();

  CType* points_to;
  void *ptr;
};

struct UntypedProxy : public Object {
  virtual void mark_node();

  UntypedProxy(void *addr);
  std::string to_string(interp::LocalRuntime &r, interp::LexicalScope &s);

  void *address;
};





} // namespace expr

#endif
