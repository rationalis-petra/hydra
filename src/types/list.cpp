#include <string>

#include "expressions.hpp"
#include "types.hpp"
#include "utils.hpp"

using std::string;

using namespace type;

void List::mark_node() {
  if (marked) return;
  Object::mark_node();
  elt_type->mark_node();
}

string List::to_string(expr::LocalRuntime &r, expr::LexicalScope &s) {
  return "{List " + hydra_to_string(elt_type, r, s) + "}";
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
