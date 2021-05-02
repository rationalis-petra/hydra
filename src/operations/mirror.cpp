#include <list>

#include "expressions.hpp"
#include "operations.hpp"
#include "utils.hpp"

using std::list;
using std::string;

using namespace expr;
using namespace op;
using namespace interp;

#include <iostream>
Object *op_get_slots(list<Object *> args, LocalRuntime &r, LexicalScope &s) {
  Mirror* mirr  = get_inbuilt<Mirror*>(args.front());
  return mirr->reflectee->get_slots(r, s);
}

Object *op_get_slot_val(list<Object *> args, LocalRuntime &r, LexicalScope &s) {
  Mirror* mirr = get_inbuilt<Mirror*>(args.front());
  args.pop_front();
  Symbol* sym = get_inbuilt<Symbol*>(args.front());
  return mirr->reflectee->get_slot_value(sym, r, s);
}

Object *op_set_slot_val(list<Object *> args, LocalRuntime &r, LexicalScope &s) {
  Mirror* mirr = get_inbuilt<Mirror*>(args.front());
  args.pop_front();
  Symbol* sym = get_inbuilt<Symbol*>(args.front());
  args.pop_front();
  Object* val = get_inbuilt<Object*>(args.front());
  return mirr->reflectee->set_slot_value(sym, val, r, s);
}

Object *op_set_slot_parent(list<Object *> args, LocalRuntime &r, LexicalScope &s) {
  Mirror* mirr = get_inbuilt<Mirror*>(args.front());
  args.pop_front();
  Symbol* sym = get_inbuilt<Symbol*>(args.front());
  mirr->reflectee->parents.insert(sym);
  args.pop_front();
  Object* bl = args.front();

  if (bl->null()) {
    mirr->reflectee->parents.erase(sym);
  } else {
    mirr->reflectee->parents.insert(sym);
  }

  return mirr;
}

Object *op_delete_slot(list<Object *> args, LocalRuntime &r, LexicalScope &s) {
  Mirror* mirr = get_inbuilt<Mirror*>(args.front());
  args.pop_front();
  Symbol* sym = get_inbuilt<Symbol*>(args.front());
  return mirr->reflectee->delete_slot(sym, r, s);
}

Object *op_get_meta(list<Object *> args, LocalRuntime &r, LexicalScope &s) {
  Mirror* mirr  = get_inbuilt<Mirror*>(args.front());
  return mirr->reflectee->get_meta(r, s);
}
Object *op_get_meta_val(list<Object *> args, LocalRuntime &r, LexicalScope &s) {
  Mirror* mirr = get_inbuilt<Mirror*>(args.front());
  args.pop_front();
  Symbol* sym = get_inbuilt<Symbol*>(args.front());
  return mirr->reflectee->get_meta_value(sym, r, s);
}
Object *op_set_meta_val(list<Object *> args, LocalRuntime &r, LexicalScope &s) {
  Mirror* mirr = get_inbuilt<Mirror*>(args.front());
  args.pop_front();
  Symbol* sym = get_inbuilt<Symbol*>(args.front());
  args.pop_front();
  Object* val = get_inbuilt<Object*>(args.front());
  return mirr->reflectee->set_meta_value(sym, val, r, s);
}
Object *op_delete_meta(list<Object *> args, LocalRuntime &r, LexicalScope &s) {
  Mirror* mirr = get_inbuilt<Mirror*>(args.front());
  args.pop_front();
  Symbol* sym = get_inbuilt<Symbol*>(args.front());
  return mirr->reflectee->delete_meta(sym, r, s);
}

Object *op_get_mirror(list<Object *> args, LocalRuntime &r, LexicalScope &s) {
  Object* obj = get_inbuilt<Object*>(args.front());
  return obj->reflect(r, s);
}

expr::GenericFn *op::get_slots;
expr::GenericFn *op::set_slot_parent;
expr::GenericFn *op::set_slot_val;
expr::GenericFn *op::get_slot_val;
expr::GenericFn *op::delete_slot;

expr::GenericFn *op::get_meta;
expr::GenericFn *op::set_meta_val;
expr::GenericFn *op::get_meta_val;
expr::GenericFn *op::delete_meta;

expr::GenericFn *op::get_mirror;

void op::initialize_mirror() {
  get_slots = new GenericFn;
  get_slots->type = type::Fn::with_args({type::mirror_type});
  get_slot_val = new GenericFn;
  get_slot_val->type = type::Fn::with_args({type::mirror_type, type::symbol_type});
  set_slot_val = new GenericFn;
  set_slot_val->type = type::Fn::with_args({type::mirror_type, type::symbol_type, new type::Any});
  set_slot_parent = new GenericFn;
  delete_slot = new GenericFn;
  delete_slot->type = type::Fn::with_args({type::mirror_type, type::symbol_type});

  get_meta = new GenericFn;
  get_meta->type = type::Fn::with_args({type::mirror_type});
  get_meta_val = new GenericFn;
  get_meta_val->type = type::Fn::with_args({type::mirror_type, type::symbol_type});
  set_meta_val = new GenericFn;
  set_meta_val->type = type::Fn::with_args({type::mirror_type, type::symbol_type, new type::Any});
  delete_meta = new GenericFn;
  delete_meta->type = type::Fn::with_args({type::mirror_type, type::symbol_type});

  get_mirror = new GenericFn;
  get_mirror->type = type::Fn::with_args({new type::Any});

  get_mirror->add(new InbuiltOperator(
                                      "reflect", "gets a mirror of the target object",
                                      op_get_mirror,
                                      type::Fn::with_args({new type::Any}),  true));

  get_slots->add(new InbuiltOperator(
      "get-slots", "gets a list of slot symbols from a mirror", op_get_slots,
      type::Fn::with_args({type::mirror_type}), true));

  get_slot_val->add(new InbuiltOperator(
      "get-slots", "gets the value a slot with a given symbol", op_get_slot_val,
      type::Fn::with_args({type::mirror_type, type::symbol_type}), true));

  set_slot_val->add(new InbuiltOperator(
      "set-slot", "sets the value of a given slot", op_set_slot_val,
      type::Fn::with_args({type::mirror_type, type::symbol_type, new type::Any}),
      true));

  delete_slot->add(new InbuiltOperator(
      "remove-slot", "removes a slot from an object", op_delete_slot,
      type::Fn::with_args({type::mirror_type, type::symbol_type}), true));





  get_meta->add(new InbuiltOperator(
      "get-metadata", "gets the list of all metadata slots from a mirror",
      op_get_meta, type::Fn::with_args({type::mirror_type}), true));

  get_meta_val->add(new InbuiltOperator(
      "get-metaslot", "gets the value of a metadata-slot with a given symbol",
      op_get_meta, type::Fn::with_args({type::mirror_type, type::symbol_type}),
      true));

  set_meta_val->add(new InbuiltOperator(
      "set-metaslot", "sets the value of a given metadata slot",
      op_set_meta_val,
      type::Fn::with_args({type::mirror_type, type::symbol_type, new type::Any}),
      true));

  delete_meta->add(new InbuiltOperator(
      "remove-metaslot", "removes a given metadata slot from an object",
      op_delete_meta,
      type::Fn::with_args({type::mirror_type, type::symbol_type}), true));

  set_slot_parent->add(new InbuiltOperator(
      "set-parent", "sets a slot to be a parent or not", op_set_slot_parent,
      type::Fn::with_args(
          {type::mirror_type, type::symbol_type, new type::Any}),
      true));
}
