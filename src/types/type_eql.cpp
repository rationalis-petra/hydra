#include <string>

#include "operations/logic.hpp"
#include "expressions.hpp"
#include "types.hpp"

using std::string;
using std::list;

type_eql::type_eql() {
  object = nullptr;
}

void type_eql::mark_node() {
  if (marked) return;
  marked = true;
  object->mark_node();
}

string type_eql::to_string() const {
  return "{Eql " + object->to_string() + "}";
}

hydra_object *type_eql::check_type(hydra_object* obj) {
  if (obj == object) {
    return hydra_t::get();
  } else {
    return hydra_nil::get();
  }
}

hydra_type *type_eql::constructor(list<hydra_object*> lst) {
  type_eql* out = new type_eql;
  if (lst.size() == 0) {
    string err = "type_eql constructor needs value to be provided";
    throw err;
  } else {
    out->object = lst.front();
  }
  return out;
}
