#include "operations.hpp"
#include "utils.hpp"

expr::Operator* op::mk_short;
expr::Operator* op::mk_long;
expr::Operator* op::mk_signed;
expr::Operator* op::mk_unsigned;

using namespace expr;
using namespace interp;

using std::list;
using std::string;

struct IntermediateModifier {
  SizeModifier size;
  SignModifier sign;

  IntermediateModifier(SizeModifier _size, SignModifier _sign) :
    size(_size), sign(_sign) {};
};

CBasicType *get_ctype(list<Object *> arg_list, LocalRuntime &r, LexicalScope &s,
                      IntermediateModifier inter) {
  CBasicType* cbasic = nullptr;

  while (!arg_list.empty()) {
    Object *obj = arg_list.front();
    if (CModifierSize *size = get_inbuilt<CModifierSize *>(obj)) {
      if (inter.size == size_none) {
        inter.size = size->size_mod;
      } else if (inter.size == size_long) {
        if (size->size_mod == size_long) {
          inter.size = size_long_long;
        } else {
          string err = "size more than once in ctype";
        }
      } else {
        string err = "size more than once in ctype";
      }
    }
    else if (CModifierSign *sign = get_inbuilt<CModifierSign *>(obj)) {
      if (inter.sign == sign_none) {
        inter.sign = sign->sign_mod;
      } else {
        string err = "sign more than once in ctype";
        throw err;
      }
    }
    else if (CBasicType *ctype = get_inbuilt<CBasicType *>(obj)) {
      if (!cbasic) {
        cbasic = new CBasicType(ctype->type);
      } else {
        string err = "more than one basic type provided to modifier";
        throw err;
      }
    } else {
      string err = "bad argument provided to c type modiifer type";
      throw err;
    }
    arg_list.pop_front();
  }

  if (cbasic) {
    cbasic->size = inter.size;
    cbasic->mod = inter.sign;

    return cbasic;
  } else {
    string err =  "C type modifier provided with no base type";
    throw err;
  }
}

Object* op_mk_short(list<Object*> arg_list, LocalRuntime& r, LexicalScope &s) {
  IntermediateModifier inter(size_short, sign_none);
  return get_ctype(arg_list, r, s, inter);
}

Object* op_mk_long(list<Object*> arg_list, LocalRuntime& r, LexicalScope &s) {
  IntermediateModifier inter(size_long, sign_none);
  return get_ctype(arg_list, r, s, inter);
}

Object* op_mk_signed(list<Object*> arg_list, LocalRuntime& r, LexicalScope &s) {
  IntermediateModifier inter(size_none, sign_yes);
  return get_ctype(arg_list, r, s, inter);
}

Object* op_mk_unsigned(list<Object*> arg_list, LocalRuntime& r, LexicalScope &s) {
  IntermediateModifier inter(size_none, sign_no);
  return get_ctype(arg_list, r, s, inter);
}

void op::initialize_foreign_proxy() {
  op::mk_short = new CModifierSize("short",
                                   "short modifier",
                                   op_mk_short,
                                   size_short);

  op::mk_long = new CModifierSize("long",
                                   "long modifier",
                                   op_mk_long,
                                   size_long);

  op::mk_signed = new CModifierSign("signed",
                                   "signed c modifier",
                                   op_mk_signed,
                                   sign_yes);

  op::mk_unsigned = new CModifierSign("unsigned", "unsigned c modifier",
                                      op_mk_unsigned, sign_no);
}
