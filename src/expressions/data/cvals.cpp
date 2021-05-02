#include <expressions/data/cvals.hpp>
#include <expressions/data/ctypes.hpp>
#include "utils.hpp"

using namespace interp;
using namespace expr;
using std::string;

CStruct::CStruct() {}

CType* CStruct::get_type() {
  return type;
}

string CStruct::to_string(LocalRuntime &r, LexicalScope &s) {
  return "<CStruct>";
}

CProxy* CStruct::get_element(Symbol* name) {
  int idx = type->type_indices[name];
  return type->types[name]->get_from_bits(data, idx, len); 
}

void* CStruct::get_ref() {
  return data;
}

  // Represetns a C pointer for FFI purposes

CPtr::CPtr(CType *_points_to, void *_ptr) :
  points_to(_points_to),
  ptr(_ptr) {
  
}

CType* CPtr::get_type() {
  return points_to;
}

void* CPtr::get_ref() {
  return &ptr;
}


// TEMPLATE SPECIALISATIONS
template <> CType *CBasic<char>::get_type() {
  return new CBasicType(tchar);
}
template <> CType *CBasic<short>::get_type() {
  CBasicType* t = new CBasicType(tint);
  t->size= size_short;
  return t;
}
template <> CType *CBasic<int>::get_type() {
  CBasicType* t = new CBasicType(tint);
  return t;
}
template <> CType *CBasic<long>::get_type() {
  CBasicType* t = new CBasicType(tint);
  t->size= size_long;
  return t;
}
template <> CType *CBasic<long long>::get_type() {
  CBasicType* t = new CBasicType(tint);
  t->size= size_long_long;
  return t;
}
template <> CType *CBasic<signed char>::get_type() {
  CBasicType* t = new CBasicType(tchar);
  t->mod = sign_yes;
  return t;
}
template <> CType *CBasic<unsigned char>::get_type() {
  CBasicType* t = new CBasicType(tchar);
  t->mod = sign_no;
  return t;
}
template <> CType *CBasic<unsigned short>::get_type() {
  CBasicType* t = new CBasicType(tint);
  t->mod = sign_no;
  t->size = size_short;
  return t;
}
template <> CType *CBasic<unsigned int>::get_type() {
  CBasicType* t = new CBasicType(tint);
  t->mod = sign_no;
  return t;
}
template <> CType *CBasic<unsigned long>::get_type() {
  CBasicType* t = new CBasicType(tint);
  t->mod = sign_no;
  t->size = size_long;
  return t;
}
template <> CType *CBasic<unsigned long long>::get_type() {
  CBasicType* t = new CBasicType(tint);
  t->mod = sign_no;
  t->size = size_long_long;
  return t;
}
template <> CType *CBasic<float>::get_type() {
  CBasicType* t = new CBasicType(tfloat);
  return t;
}
template <> CType *CBasic<double>::get_type() {
  CBasicType* t = new CBasicType(tdouble);
  return t;
}
template <> CType *CBasic<long double>::get_type() {
  CBasicType* t = new CBasicType(tdouble);
  t->size = size_long;
  return t;
}
