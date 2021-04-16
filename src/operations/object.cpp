#include "operations/logic.hpp"
#include "operations/data.hpp"
#include "utils.hpp"

using std::list;
using std::string;

using namespace expr;
using namespace interp;


Object *op_mk_object(list<Object *> arg_list, LocalRuntime &r,
                     LexicalScope &s) {
  Object *object = new Object;

  // Default behaviour
  Symbol *defbeh = get_keyword("default-behaviour");
  object->slots[defbeh] = Object::default_behaviour;

  for (Object *kvp : arg_list) {
    Cons *cns = get_inbuilt<Cons *>(kvp);
    list<Object *> slot_descriptor = cons_to_list(cns);
    Symbol *slot = get_inbuilt<Symbol *>(slot_descriptor.front());
    slot_descriptor.pop_front();
    Object *value = get_inbuilt<Object *>(slot_descriptor.front());
    slot_descriptor.pop_front();

    bool is_parent = false;

    for (Object *obj : slot_descriptor) {
      if (Cons *cns = get_inbuilt<Cons *>(obj)) {
        Symbol *sym = get_inbuilt<Symbol *>(cns->car);
        list<Object *> args = cons_to_list(cns->cdr);

        if (sym == get_keyword("parent")) {
          is_parent = true;
        } else if (sym == get_keyword("writer")) {
        } else {
        }
      } else if (Symbol *sym = get_inbuilt<Symbol *>(obj)) {
        if (sym == get_keyword("parent")) {
          is_parent = true;

        } else if (sym == get_keyword("accessor") ||
                   sym == get_keyword("reader") ||
                   sym == get_keyword("writer")) {
          if (!slot->invoker) {
            GenericFn *gfn = new GenericFn;
            slot->invoker = gfn;
            gfn->type =
                type::Fn::with_args_optional({new type::Any}, {new type::Any});

            if (sym == get_keyword("accessor")) {
              gfn->add(new Reader(object, slot));
              gfn->add(new Writer(object, slot));
            }
            else if (sym == get_keyword("reader")) {
              gfn->add(new Reader(object, slot));
            }
            else if (sym == get_keyword("writer")) {
              gfn->add(new Writer(object, slot));
            } else {
              string err = "Incorrect argument to mk_object";
              throw err;
            }
          }

        } else {
        }
      } else {
        string err = "Incorrect argument to mk_object";
        throw err;
      }
    }

    object->slots[slot] = value;
    if (is_parent) {
      object->parents.insert(slot);
    }
  }

  return object;
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



GenericFn *op::mk_obj;
GenericFn *op::clone;

void op::initialize_user_obj() {
  op::mk_obj = new GenericFn;
  op::mk_obj->type = type::Fn::with_all({}, new type::Cons, new type::Any);
  op::clone = new GenericFn;
  op::clone->type = type::Fn::with_rest(new type::Any);

  op::mk_obj->add(new InbuiltOperator(
      "obj", "Create a new object", op_mk_object,
      type::Fn::with_all({}, new type::Cons, new type::Any), true));


  op::clone->add(
      new InbuiltOperator("clone", "Derives an object from the provided object",
                          op_clone, type::Fn::with_rest(new type::Any), true));

  op::bin_equal->add(new InbuiltOperator(
      "object =", "Equality test for Objects", op_obj_eq,
      type::Fn::with_args({new type::Any, new type::Any}), true));
}
