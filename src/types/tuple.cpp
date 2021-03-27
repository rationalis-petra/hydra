#include <string>

#include "utils.hpp"
#include "expressions.hpp"
#include "types.hpp"
#include "operations/types.hpp"

using std::string;
using std::list;

using namespace type;
using namespace interp;

Tuple::Tuple() {
  invoker = op::mk_tuple_type;
}

Tuple::Tuple(std::vector<Type *> _types) : types(_types) {
  invoker = op::mk_tuple_type;
}
  

void Tuple::mark_node() {
  if (marked) return;
  Object::mark_node();
  for (Type* t : types) {
    t->mark_node();
  }
}

string Tuple::to_string(LocalRuntime &r, LexicalScope &s) {
  string str =  "{Tuple";
  for (Type* t : types) {
    str += " " + hydra_to_string(t, r, s);
  }
  str += "}";
  return str;
}

expr::Object *Tuple::check_type(expr::Object* obj) {
  if (expr::Tuple* tup = dynamic_cast<expr::Tuple*>(obj)) {
    if (types.size() == 0) {
      return expr::t::get();
    }
    if (tup->values.size() != types.size()) {
      return expr::nil::get();
    }
    for (unsigned i = 0 ; i < types.size(); i++) {
      if (types[i]->check_type(tup->values[i])->null()) {
        return expr::nil::get();
      }
    }
    return expr::t::get();
  } else {
    return expr::nil::get();
  }
}


expr::Object *Tuple::subtype(Type * ty) {
  if (Tuple* other = dynamic_cast<Tuple*>(ty)) {
    if (other->types.size() != types.size()) {
      return expr::nil::get();
    } else {
      for (unsigned i = 0; i < types.size(); i++) {
        if (!types[i]->subtype(other->types[i])) {
          return expr::nil::get();
        }
      }
      return expr::t::get();
    }
  } else {
    return expr::nil::get();
  }
}

expr::Object* op_mk_tuple(list<expr::Object*> lst, LocalRuntime &r, LexicalScope& s) {
  Tuple* tup = new Tuple;
  for (expr::Object* obj : lst) {
    if (Type *t = dynamic_cast<Type*>(obj)) {
      tup->types.push_back(t);
    }
  }
  return tup;
}


