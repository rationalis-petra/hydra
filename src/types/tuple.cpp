#include <string>

#include "expressions.hpp"
#include "types.hpp"

using std::string;
using std::list;

using namespace type;

Tuple::Tuple(){};
Tuple::Tuple(std::vector<Type*> _types) : types(_types) {}
  

void Tuple::mark_node() {
  marked = true;
}

string Tuple::to_string() const {
  string str =  "{Tuple";
  for (Type* t : types) {
    str += " " + t->to_string();
  }
  str += "}";
  return str;
}

expr::Value *Tuple::check_type(expr::Value* obj) {
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

Type *Tuple::constructor(list<expr::Value*> lst) {
  Tuple * tup = new Tuple;
  for (expr::Value* obj : lst) {
    if (Type *t = dynamic_cast<Type*>(obj)) {
      tup->types.push_back(t);
    }
  }
  return tup;
}
