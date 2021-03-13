#include "operations/data.hpp"

using std::list;
using std::string;

using namespace expr;

Value* op_obj_get(Operator* op, Value* alist, LocalRuntime &r, LexicalScope& s) {
  list<Value*> arg_list = op->get_arg_list(alist, r, s);

  Object* obj = dynamic_cast<Object*>(arg_list.front());
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

Value* op_mk_object(Operator* op, Value* alist, LocalRuntime &r, LexicalScope& s) {
  list<Value*> arg_list = op->get_arg_list(alist, r, s);
  Object* obj = new Object;

  for (Value* kvp : arg_list) {
    Tuple* tup = dynamic_cast<Tuple*>(kvp);
    Symbol* slot = dynamic_cast<Symbol*>(tup->values[0]);
    Value* value = dynamic_cast<Value*>(tup->values[1]);
    obj->object_vals[slot] = value;
  }

  return obj;
}

Operator* op::mk_obj =
  new InbuiltOperator("Create a new object",
                      op_mk_object, type::Fn::with_all({}, new type::Tuple({new type::Symbol, new type::Any}), new type::Object), true);

Value* op_obj_set(Operator* op, Value* alist, LocalRuntime &r, LexicalScope& s) {
  list<Value*> arg_list = op->get_arg_list(alist, r, s);

  Object* obj = dynamic_cast<Object*>(arg_list.front());
  arg_list.pop_front();
  Symbol* sym = dynamic_cast<Symbol*>(arg_list.front());
  Value* newval = arg_list.back();

  obj->object_vals[sym] = newval;
  return newval;
}

Operator* op::obj_set =
  new InbuiltOperator("Set a particular slot in an object to a vlue",
                      op_obj_set,
                      type::Fn::with_args({new type::Object, new type::Symbol, new type::Any}),
                      true);
Value* op_derive(Operator* op, Value* alist, LocalRuntime &r, LexicalScope& s) {
  list<Value*> arg_list = op->get_arg_list(alist, r, s);
  Object* obj = new Object;
  Object* prototype = dynamic_cast<Object*>(arg_list.front());

  obj->prototypes.insert(prototype);
  obj->object_vals = prototype->object_vals;

  return obj;
}

Operator* op::derive =
  new InbuiltOperator("Derives an object from the provided object",
                      op_derive, type::Fn::with_rest(new type::Any), true);


