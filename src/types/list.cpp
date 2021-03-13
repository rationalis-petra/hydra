
#include <string>

#include "expressions.hpp"
#include "types.hpp"

using std::string;
using std::list;

using namespace type;

void List::mark_node() {
  marked = true;
}

string List::to_string() const {
  return "{List " + elt_type->to_string() + "}";
}

expr::Value *List::check_type(expr::Value* obj) {
  if ((dynamic_cast<expr::Cons*>(obj)) || obj->null()) {
    return expr::t::get();
  } else {
    return expr::nil::get();
  }
}

Type *List::constructor(list<expr::Value*> lst) {
  List* ret = new List();

  if (lst.size() == 0) {
    ret->elt_type = new Any;
    return ret;
  } else if (lst.size() == 1) {
    if (Type* tp = dynamic_cast<Type*>(lst.front())) {
      ret->elt_type = tp;
      return ret;
    } else {
      string err = "List requires only one argument";
      throw err;
    }
  } else {
    string err = "too many arguments provided to Type List constructor";
    throw err;
  }
}
