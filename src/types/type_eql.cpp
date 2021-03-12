#include <string>

#include "operations/logic.hpp"
#include "expressions.hpp"
#include "types.hpp"

using std::string;
using std::list;

using type::Type;
using type::Eql;
using type::EqlConstructor;
using expr::Value;

Eql::Eql(Value* v) {
  object = v;
}

void Eql::mark_node() {
  if (marked) return;
  marked = true;
  object->mark_node();
}

string Eql::to_string() const {
  return "{Eql " + object->to_string() + "}";
}

Value *Eql::check_type(Value* obj) {
  if (obj == object) {
    return expr::t::get();
  } else {
    return expr::nil::get();
  }
}

void EqlConstructor::mark_node() {
  marked = true;
}

Type* EqlConstructor::constructor(list<Value*> lst) {
  if (lst.size() == 0) {
    string err = "Eql constructor needs value to be provided";
    throw err;
  } else {
    return new Eql(lst.front());
  }
}

string EqlConstructor::to_string() const {
  return "Constructor for the Eql type";
}
