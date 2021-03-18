
#include <string>

#include "expressions.hpp"
#include "types.hpp"

using std::string;

using namespace type;

void List::mark_node() {
  Object::mark_node();
}

string List::to_string() const {
  return "{List " + elt_type->to_string() + "}";
}

expr::Object *List::check_type(expr::Object* obj) {
  if ((dynamic_cast<expr::Cons*>(obj)) || obj->null()) {
    return expr::t::get();
  } else {
    return expr::nil::get();
  }
}

expr::Object *List::subtype(Type* other) {
  if (List* lst = dynamic_cast<List*>(other)) {
    return elt_type->subtype(lst->elt_type);
  } else {
    return expr::nil::get();
  }
}
