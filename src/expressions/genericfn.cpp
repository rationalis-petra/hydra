#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <typeinfo>

#include "utils.hpp"
#include "expressions.hpp"
#include "operations.hpp"

using std::list;
using std::string;

using namespace expr;
using namespace interp;

// GENERIC FUNCTION

GenericFn::GenericFn(string name) {
  is_fn = true;
  type->rest_type = nullptr;
  type->return_type = new type::Any;

  metadata[get_keyword("name")] = new HString(name);
}

GenericFn::GenericFn() {
  is_fn = true;
  type->rest_type = nullptr;
  type->return_type = new type::Any;
}

string GenericFn::to_string(LocalRuntime &r, LexicalScope &s) {
  auto it = metadata.find(get_keyword("name"));

  if (it != metadata.end()) {
    return "<gfn: " + hydra_to_string(it->second, r, s) + ">";
  } else {
    return "<anonymous gfn>";
  }
}

void GenericFn::mark_node() {
  if (marked)
    return;
  mark_node();

  marked = true;
  type->mark_node();
  for (Operator *o : functions) {
    o->mark_node();
  }
}


void GenericFn::add(Operator *fn) {
  if (GenericFn *f = dynamic_cast<GenericFn *>(fn)) {
    // is this the best??
    for (Operator *o : f->functions) {
      add(o);
    }
  } else if (fn->is_fn) {
    functions.push_front(fn);
  } else {
    string err = "Attempted to add macro to add-fn!";
    throw err;
  }
}

void GenericFn::add_safe(Operator *fn, LocalRuntime& r, LexicalScope& s) {
  if (GenericFn *f = dynamic_cast<GenericFn *>(fn)) {
    // is this the best??
    for (Operator *o : f->functions) {
      add_safe(o, r, s);
    }
  } else if (fn->is_fn) {
    auto it = functions.begin();
    while (it != functions.end()) {
      if (!equal_operator->call({fn->type,(*it)->type}, r, s)->null())
        functions.erase(it);
      it++;
    }
    functions.push_front(fn);
  } else {
    string err = "Attempted to add macro to add-fn!";
    throw err;
  }
}

Object *GenericFn::call(list<Object*> arg_list, LocalRuntime &r, LexicalScope &s, bool) {
  // ASSUME that the function itself is marked

  // first find a list of applicable functions
  std::list<Operator*> applicables;
  for (Operator *fn : functions) {
    try {
      if (!fn->type->check_args(arg_list)->null()) {
        applicables.push_back(fn);

        // we need to unroot values in the arg_list
      }
    } catch (TypeCheckException *e) {}
  }

  // Now, we have a list of applicable functions, we need to support them
  // using the subtype relation
 applicables.sort([](Operator *op1, Operator *op2) {
    return !(op1->type->subtype(op2->type))->null();
  });

  if (applicables.empty()) {
    string err = "no applicable method found in generic function" +
                 hydra_to_string(this, r, s);
    throw err;
  } else if (applicables.size() > 2) {
    auto it = applicables.begin();
    Operator* op1 = *it++;
    Operator* op2 = *it;
    if (!op2->type->subtype(op1->type)->null()) {
      string err = "Ambiguous call in generic function";
      throw err;
    }
  }

  // now we have a sorted list, we just need to execute the methods, one-by-one
  // only executing the next method if the previous one executed
  // in addition, we need to introduce the 'call-next' function into
  // the lexical scope
  // } else {
    Symbol *call_next = core_module->intern("call-next");
    Operator *op = applicables.front();
    applicables.pop_front();
    NextFnc *nextfnc = new NextFnc(applicables, arg_list, call_next);

    UserOperator* uop = nullptr;
    if ((uop = dynamic_cast<UserOperator*>(op))) {
      uop->scope->map[call_next] = nextfnc;
    }
    Object* out = op->call(arg_list, r, s);
    for (Object *obj : arg_list) {
      collector->remove_root(obj);
    }
    if (uop) {
      uop->scope->map.erase(call_next);
    }
    return out;
    //}
}

NextFnc::NextFnc(list<Operator*> _fncs, list<Object*> _arg_list, Symbol* _nextsym) {
  funcs = _fncs;
  arg_list = _arg_list;
  nextsym = _nextsym;
}

#include <iostream>
Object* NextFnc::call(std::list<Object*> arglist, LocalRuntime& r, LexicalScope& s, bool) {
  Operator* op = funcs.front();
  funcs.pop_front();
  UserOperator *uop = nullptr;
  if ((uop = dynamic_cast<UserOperator*>(op))) {
    uop->scope->map[nextsym] = this;
  }
  Object* out = op->call(arg_list, r, s);
  if (uop) {
      uop->scope->map.erase(nextsym);
  }
  return out;
}
