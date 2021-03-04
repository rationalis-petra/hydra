#include "operations/data.hpp"

using std::list;
using std::string;

op_obj_get::op_obj_get() {
  is_fn = true;
  docstring = new hydra_string("Get a slot from an object");
  type->arg_list.push_front(new type_symbol);
  type->arg_list.push_front(new type_nil);
}

hydra_object* op_obj_get::call(hydra_object* alist, runtime &r, lexical_scope& s) {
  list<hydra_object*> arg_list = get_arg_list(alist, r, s);

  hydra_object_object* obj = dynamic_cast<hydra_object_object*>(arg_list.front());
  hydra_symbol* sym = dynamic_cast<hydra_symbol*>(arg_list.back());

  try {
    return obj->object_vals.at(sym);
  } catch (std::out_of_range& e) {
    string err = "Attempt to get non-existent slot in object: " + sym->to_string();
    throw err;
  }
}

op_object::op_object() {
  is_fn = true;
  docstring = new hydra_string("A generic getter function");
  type->rest_type = new type_nil;
}

hydra_object* op_object::call(hydra_object* alist, runtime &r, lexical_scope& s) {
  list<hydra_object*> arg_list = get_arg_list(alist, r, s);
  hydra_object_object* obj = new hydra_object_object;

  int count = 0;
  hydra_symbol* key;
  for (hydra_object* kvp : arg_list) {
    if (count == 0) {
      key = dynamic_cast<hydra_symbol*>(kvp);
      if (key == nullptr) {
        string err = "key was nullptr in op_object";
        throw err;
      }
      count++;
    } else {
      count--;
      obj->object_vals[key] = kvp;
    }
  }

  return obj;
}
