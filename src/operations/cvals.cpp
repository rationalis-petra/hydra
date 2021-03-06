#include "operations.hpp"
#include "utils.hpp"

expr::Operator* op::mk_short;
expr::Operator* op::mk_long;
expr::Operator* op::mk_signed;
expr::Operator* op::mk_unsigned;
expr::Operator *op::mk_cfntype;

expr::Operator* op::mk_proxy;
expr::Operator* op::mk_ptrtype;
expr::Operator* op::mk_enumtype;

using namespace expr;
using namespace interp;

using std::list;
using std::string;
using std::unordered_map;

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

Object* op_mk_proxy(list<Object*> arg_list, LocalRuntime& r, LexicalScope &s) {
  CType* ctype = get_inbuilt<CType*>(arg_list.front());
  arg_list.pop_front();
  Object* obj = arg_list.front();

  if (!ctype) {
    return new HString("bad ctype!");
  }

  return ctype->get_from_object(obj);
}

Object* op_mk_cfntype(list<Object*> arg_list, LocalRuntime& r, LexicalScope &s) {
  CType* rettype = get_inbuilt<CType*>(arg_list.front());
  arg_list.pop_front();
  list<Object*> args = cons_to_list(arg_list.front());
  list<CType*> typeargs;
  for (auto arg : args) {
    typeargs.push_back(get_inbuilt<CType*>(arg));

  }

  return new CFnType(rettype, typeargs);
}


Object* op_mk_cptrtype(list<Object*> arg_list, LocalRuntime& r, LexicalScope &s) {
  CType* type = get_inbuilt<CType*>(arg_list.front());
  return new CPtrType(type);
}

Object* op_mk_enumtype(list<Object*> arg_list, LocalRuntime& r, LexicalScope &s) {
  unordered_map<Symbol*, int> map;
  long counter = 0;
  for (Object* obj : arg_list) {
    if (Symbol* sym = get_inbuilt<Symbol*>(obj)) {
      map[sym] = counter++;
    } else if (Cons* cns = get_inbuilt<Cons*>(obj)) {
      Symbol* sym = get_inbuilt<Symbol*>(cns->car);
      cns = get_inbuilt<Cons*>(cns->cdr);
      if (sym && cns) {
        Integer* i = get_inbuilt<Integer*>(cns->car);
        counter = i->get_sl();
        map[sym] = counter++;
      } 
    } else {
      string err = "bad value to mk_enumtype";
      throw err;
    }
  }
  return new CEnumType(map);
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

  op::mk_proxy = new InbuiltOperator(
      "proxy", "make a C proxy object",
      op_mk_proxy,
      type::Fn::with_args({type::c_type_type, new type::Any}),
      true);

  op::mk_cfntype = new InbuiltOperator(
      "fn", "make a C function type",
      op_mk_cfntype,
      type::Fn::with_args({type::c_type_type, new type::List()}),
      true);

  op::mk_ptrtype = new InbuiltOperator(
      "ptr", "make a C pointer type",
      op_mk_cptrtype,
      type::Fn::with_args({type::c_type_type}),
      true);

  op::mk_enumtype = new InbuiltOperator(
      "enum", "make a C enum type",
      op_mk_enumtype,
      type::Fn::with_rest(new type::Any),
      true);
}
