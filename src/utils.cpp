#include "utils.hpp"

using namespace expr;
using std::list;
using std::string;

std::list<Object*> cons_to_list(Object* v) {
  list<Object*> out;
  while (!v->null()) {
    Cons* cns = dynamic_cast<Cons*>(v);
    if (!cns) {
      std::string err = "Interperter Error in cons_to_list:  " + v->to_string() + " is not a nil or a cons";
      throw err;
    }
    out.push_back(cns->car);
    v = cns->cdr;
  }
  return out;
}

Object* list_to_cons(std::list<Object*> lst) {
  if (lst.empty()) {
    return (Object*) expr::nil::get();
  } else {
    Object* car = lst.front();
    lst.pop_front();
    return (Object*) new expr::Cons(car, list_to_cons(lst));
  }
}

Symbol* get_keyword(string str) {
  Object* obj = keyword_module->get(str);
  if (obj->null()) {
    Symbol* sym = keyword_module->intern(str);
    sym->value = (Object*) sym;
    return sym;
  }
  else {
    return dynamic_cast<Symbol*>(obj);
  }
}