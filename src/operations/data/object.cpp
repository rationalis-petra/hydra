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
    Tuple *tup = get_inbuilt<Tuple *>(kvp);
    Symbol *slot = get_inbuilt<Symbol *>(tup->values[0]);
    Object *value = get_inbuilt<Object *>(tup->values[1]);
    Object *has_getter = get_inbuilt<Object *>(tup->values[2]);
    Object *has_setter = get_inbuilt<Object *>(tup->values[3]);
    Object *is_parent = get_inbuilt<Object *>(tup->values[4]);

    obj->slots[slot] = value;
    if (!is_parent->null()) {
      obj->parents.insert(slot);
    }
    if (!has_getter->null()) {
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

GenericFn *op::obj_get;
GenericFn *op::mk_obj;
GenericFn *op::obj_set;
GenericFn *op::clone;
GenericFn *op::obj_eq;
GenericFn *op::obj_pset;

void op::initialize_user_obj() {
  op::obj_get = new GenericFn;
  op::obj_get->type = type::Fn::with_args({new type::Any, new type::Symbol});
  op::mk_obj = new GenericFn;
  op::mk_obj->type = type::Fn::with_all(
      {},
      new type::Tuple({new type::Symbol, new type::Any, new type::Any,
                       new type::Any, new type::Any}),
      new type::Any);
  op::obj_set = new GenericFn;
  op::obj_set->type =
      type::Fn::with_args({new type::Any, new type::Symbol, new type::Any});
  op::clone = new GenericFn;
  op::clone->type = type::Fn::with_rest(new type::Any);
  op::obj_eq = new GenericFn;
  op::obj_eq->type = type::Fn::with_args({new type::Any, new type::Any});
  op::obj_pset = new GenericFn;
  op::obj_pset->type = type::Fn::with_args({new type::Symbol, new type::Any});

  op::obj_get->add(new InbuiltOperator(
      "get", "Get a slot from an object", op_obj_get,
      type::Fn::with_args({new type::Any, new type::Symbol}), true));

  op::mk_obj->add(new InbuiltOperator(
      "obj", "Create a new object", op_mk_object,
      type::Fn::with_all(
          {},
          new type::Tuple({new type::Symbol, new type::Any, new type::Any,
                           new type::Any, new type::Any}),
          new type::Any),
      true));

  op::obj_set->add(new InbuiltOperator(
      "set", "Set a particular slot in an object to a vlue", op_obj_set,
      type::Fn::with_args({new type::Any, new type::Symbol, new type::Any}),
      true));

  op::clone->add(
      new InbuiltOperator("clone", "Derives an object from the provided object",
                          op_clone, type::Fn::with_rest(new type::Any), true));

  op::obj_eq->add(new InbuiltOperator(
      "object =", "Equality test for Objects", op_obj_eq,
      type::Fn::with_args({new type::Any, new type::Any}), true));

  op::obj_pset->add(new InbuiltOperator(
      "parent-set", "Sets provided slot to a parent slot", op_obj_eq,
      type::Fn::with_args({new type::Symbol, new type::Any}), true));
}
