#include "operations/data.hpp"

using std::list;
using std::string;

using namespace expr;

Object* op_obj_get(list<Object*> arg_list, LocalRuntime &r, LexicalScope& s) {
  

  UserObject* obj = dynamic_cast<UserObject*>(arg_list.front());
  Symbol* sym = dynamic_cast<Symbol*>(arg_list.back());

  try {
    return obj->object_vals.at(sym);
  } catch (std::out_of_range& e) {
    string err = "Attempt to get non-existent slot in object: " + sym->to_string();
    throw err;
  }
}

Operator* op::obj_get =
  new InbuiltOperator("Get a slot from an object", op_obj_get,
                      type::Fn::with_args({new type::Any, new type::Symbol}), true);

Object* op_mk_object(list<Object*> arg_list, LocalRuntime &r, LexicalScope& s) {
  
  UserObject* obj = new UserObject;

  for (Object* kvp : arg_list) {
    Tuple* tup = dynamic_cast<Tuple*>(kvp);
    Symbol* slot = dynamic_cast<Symbol*>(tup->values[0]);
    Object* value = dynamic_cast<Object*>(tup->values[1]);
    obj->object_vals[slot] = value;
  }

  return obj;
}

Operator* op::mk_obj =
  new InbuiltOperator("Create a new object",
                      op_mk_object, type::Fn::with_all({}, new type::Tuple({new type::Symbol, new type::Any}), new type::Any), true);

Object* op_obj_set(list<Object*> arg_list, LocalRuntime &r, LexicalScope& s) {
  

  UserObject* obj = dynamic_cast<UserObject*>(arg_list.front());
  arg_list.pop_front();
  Symbol* sym = dynamic_cast<Symbol*>(arg_list.front());
  Object* newval = arg_list.back();

  obj->object_vals[sym] = newval;
  return newval;
}

Operator* op::obj_set =
  new InbuiltOperator("Set a particular slot in an object to a vlue",
                      op_obj_set,
                      type::Fn::with_args({new type::Any, new type::Symbol, new type::Any}),
                      true);
Object* op_derive(list<Object*> arg_list, LocalRuntime &r, LexicalScope& s) {
  
  UserObject* obj = new UserObject;
  UserObject* prototype = dynamic_cast<UserObject*>(arg_list.front());

  obj->prototypes.insert(prototype);
  obj->object_vals = prototype->object_vals;

  return obj;
}

Operator* op::derive =
  new InbuiltOperator("Derives an object from the provided object",
                      op_derive, type::Fn::with_rest(new type::Any), true);


