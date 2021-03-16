#include <string>
#include <list>

#include "expressions.hpp"
#include "types.hpp"

using std::string;
using std::list;

using namespace type;

void Cons::mark_node() {
  Object::mark_node();
}

string Cons::to_string() const {
  return "Cons";
}

expr::Object *Cons::check_type(expr::Object* obj) {
  if ((dynamic_cast<expr::Cons*>(obj)) == nullptr) {
    return expr::nil::get();
  } else {
    return expr::t::get();
  }
}

Type *Cons::constructor(list<Object*> lst) {
  if (lst.size() == 0) {
    return this;
  } else {
    string err = "Cannot provide argument to the Char Type constructor";
    throw err;
  }
}

expr::Object *Cons::subtype(Type *obj) {
  if (Cons *tcons = dynamic_cast<Cons*>(obj)) {
    // if (type_car->subtype(tcons->type_car) &&
    //     type_cdr->subtype(tcons->type_cdr))
    return expr::t::get();
  }
  return expr::nil::get();
}
