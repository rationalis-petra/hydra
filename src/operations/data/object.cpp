#include "operations/data.hpp"
#include "utils.hpp"

using std::list;
using std::string;

using namespace expr;
using namespace interp;

Object *op_obj_get(list<Object *> arg_list, LocalRuntime &r, LexicalScope &s) {

  Object *obj = arg_list.front();
  Symbol *sym = dynamic_cast<Symbol *>(arg_list.back());

  try {
    return obj->slots.at(sym);
  } catch (std::out_of_range &e) {
    string err = "Attempt to get non-existent slot in object: " +
                 hydra_to_string(sym, r, s);
    throw err;
  }
}

Object *op_mk_object(list<Object *> arg_list, LocalRuntime &r,
                     LexicalScope &s) {

  Object *obj = new Object;

  for (Object *kvp : arg_list) {
    Tuple *tup = dynamic_cast<Tuple *>(kvp);
    Symbol *slot = dynamic_cast<Symbol *>(tup->values[0]);
    Object *value = dynamic_cast<Object *>(tup->values[1]);
    Object *is_parent = dynamic_cast<Object *>(tup->values[1]);
    obj->slots[slot] = value;
    if (!is_parent->null()) {
      obj->parents.insert(slot);
    }
  }

  return obj;
}

Object *op_obj_set(list<Object *> arg_list, LocalRuntime &r, LexicalScope &s) {

  Object *obj = dynamic_cast<Object *>(arg_list.front());
  arg_list.pop_front();
  Symbol *sym = dynamic_cast<Symbol *>(arg_list.front());
  Object *newval = arg_list.back();

  obj->slots[sym] = newval;
  return newval;
}

Object *op_clone(list<Object *> arg_list, LocalRuntime &r, LexicalScope &s) {

  Object *obj = new Object;
  Object *clonee = dynamic_cast<Object *>(arg_list.front());

  obj->slots = clonee->slots;

  return obj;
}

Object *op_obj_eq(list<Object *> arg_list, LocalRuntime &r, LexicalScope &s) {

  Object *obj1 = dynamic_cast<Object *>(arg_list.front());
  Object *obj2 = dynamic_cast<Object *>(arg_list.back());

  if (obj1 == obj2) {
    return t::get();
  }
  return nil::get();
}

Object *op_obj_pset(list<Object *> arg_list, LocalRuntime &r, LexicalScope &s) {
  Symbol *sym = dynamic_cast<Symbol *>(arg_list.front());
  Object *obj = dynamic_cast<Object *>(arg_list.back());

  if (obj->slots.find(sym) != obj->slots.end()) {
    obj->parents.insert(sym);
    return obj;
  } else {
    string err = "Cannot set as parent non-existent slot";
    throw err;
  }
}

Operator *op::obj_get;
Operator *op::mk_obj;
Operator *op::obj_set;
Operator *op::clone;
Operator *op::obj_eq;
Operator *op::obj_pset;

void op::initialize_user_obj() {

  op::obj_get = new InbuiltOperator(
      "get", "Get a slot from an object", op_obj_get,
      type::Fn::with_args({new type::Any, new type::Symbol}), true);
  op::mk_obj = new InbuiltOperator(
      "obj", "Create a new object", op_mk_object,
      type::Fn::with_all(
          {}, new type::Tuple({new type::Symbol, new type::Any, new type::Any}),
          new type::Any),
      true);
  op::obj_set = new InbuiltOperator(
      "set", "Set a particular slot in an object to a vlue", op_obj_set,
      type::Fn::with_args({new type::Any, new type::Symbol, new type::Any}),
      true);
  op::clone =
      new InbuiltOperator("clone", "Derives an object from the provided object",
                          op_clone, type::Fn::with_rest(new type::Any), true);

  op::obj_eq = new InbuiltOperator(
      "object =", "Equality test for Objects", op_obj_eq,
      type::Fn::with_args({new type::Any, new type::Any}), true);

  op::obj_pset = new InbuiltOperator(
      "parent-set", "Sets provided slot to a parent slot", op_obj_eq,
      type::Fn::with_args({new type::Symbol, new type::Any}), true);
}
