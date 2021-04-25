#include <expressions/data/struct.hpp>
#include "utils.hpp"

using namespace interp;
using namespace expr;
using std::string;
using std::list;
using std::pair;

// struct CProxy : Object {
//   CType* type;
//   
// };

// template <typename T> struct CBasic : CProxy {
//   CBasic(T val) {
//     value = val;
//   }

//   CBasic(unsigned char* arr, int idx, int len) {
//     unsigned char out[sizeof(T)];
//     if (idx + sizeof(T) > len) {
//       std::string err = "attempt to construct CPoxy from bit-array would go off end of bit-array";
//       throw err;
//     }
//     for (int i = 0; i < sizeof(T); i++) {
//       out[i] = arr[idx + i];
//     }
//     //value = (T) out;
//   };
//   T value;
// };

// struct CStruct : public CProxy {
//   CStruct();
//   CStructType* type;
//   std::string to_string(interp::LocalRuntime &r, interp::LexicalScope& s);

//   unsigned char* data;

//   // a data structure enabling the retrieval of data from  
//   std::map<Symbol*, CType*>
//   get_element(Symbol* name);
// };

CStruct::CStruct() {}

string CStruct::to_string(LocalRuntime &r, LexicalScope &s) {}

CProxy* CStruct::get_element(Symbol* name) {
  int idx = type->type_indices[name];
  return type->types[name]->get_from_bits(data, idx, len); 
}


  // Represetns a C pointer for FFI purposes
// struct CPtr : public CProxy {
//   CType* points_to;
//   void *ptr;
// };
