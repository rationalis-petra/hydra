#include <string>
#include <list>

#include "expressions.hpp"
#include "types.hpp"

using std::string;
using std::list;

using namespace type;

void Cons::mark_node() {
  marked = true;
}

string Cons::to_string() const {
  return "Cons";
}

expr::Value *Cons::check_type(expr::Value* obj) {
  if ((dynamic_cast<expr::Cons*>(obj)) == nullptr) {
    return expr::nil::get();
  } else {
    return expr::t::get();
  }
}

Type *Cons::constructor(list<Value*> lst) {
  if (lst.size() == 0) {
    return this;
  } else {
    string err = "Cannot provide argument to the Char Type constructor";
    throw err;
  }
}
