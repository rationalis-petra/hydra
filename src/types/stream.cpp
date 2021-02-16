
#include <string>

#include "expressions.hpp"
#include "types.hpp"

using std::string;
using std::list;

// INPUT-OUTPUT STREAM
void type_iostream::mark_node() {
  marked = true;
}

string type_iostream::to_string() const {
  return "IOStream";
}

hydra_object *type_iostream::check_type(hydra_object* obj) {
  if ((dynamic_cast<hydra_iostream*>(obj)) == nullptr) {
    return hydra_nil::get();
  } else {
    return hydra_t::get();
  }
}

hydra_type *type_iostream::constructor(list<hydra_object*> lst) {
  if (lst.size() == 0) {
    return this;
  } else {
    string err = "Cannot provide argument to the IOStream Type constructor";
    throw err;
  }
}

// INPUT STREAM
void type_istream::mark_node() {
  marked = true;
}

string type_istream::to_string() const {
  return "IStream";
}

hydra_object *type_istream::check_type(hydra_object* obj) {
  if ((dynamic_cast<hydra_istream*>(obj)) == nullptr &&
      (dynamic_cast<hydra_iostream*>(obj)) == nullptr) {
    return hydra_nil::get();
  } else {
    return hydra_t::get();
  }
}

hydra_type *type_istream::constructor(list<hydra_object*> lst) {
  if (lst.size() == 0) {
    return this;
  } else {
    string err = "Cannot provide argument to the IStream Type constructor";
    throw err;
  }
}

// OUTPUT STREAM
void type_ostream::mark_node() {
  marked = true;
}

string type_ostream::to_string() const {
  return "OStream";
}

hydra_object *type_ostream::check_type(hydra_object* obj) {
  if ((dynamic_cast<hydra_ostream*>(obj)) == nullptr &&
      (dynamic_cast<hydra_iostream*>(obj)) == nullptr) {
    return hydra_nil::get();
  } else {
    return hydra_t::get();
  }
}

hydra_type *type_ostream::constructor(list<hydra_object*> lst) {
  if (lst.size() == 0) {
    return this;
  } else {
    string err = "Cannot provide argument to the OStream Type constructor";
    throw err;
  }
}
