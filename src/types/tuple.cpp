#include <string>

#include "expressions.hpp"
#include "types.hpp"

using std::string;
using std::list;

void type_tuple::mark_node() {
  marked = true;
}

string type_tuple::to_string() const {
  string str =  "{Tuple";
  for (hydra_type* t : types) {
    str += " " + t->to_string();
  }
  str += "}";
  return str;
}

hydra_object *type_tuple::check_type(hydra_object* obj) {
  if (hydra_tuple* tup = dynamic_cast<hydra_tuple*>(obj)) {
    if (types.size() == 0) {
      return hydra_t::get();
    }
    if (tup->values.size() != types.size()) {
      return hydra_nil::get();
    }
    for (unsigned i = 0 ; i < types.size(); i++) {
      if (types[i]->check_type(tup->values[i])->null()) {
        return hydra_nil::get();
      }
    }
    return hydra_t::get();
  } else {
    return hydra_nil::get();
  }
}

hydra_type *type_tuple::constructor(list<hydra_object*> lst) {
  type_tuple * tup = new type_tuple;
  for (hydra_object* obj : lst) {
    if (hydra_type *t = dynamic_cast<hydra_type*>(obj)) {
      tup->types.push_back(t);
    }
  }
  return tup;
}
