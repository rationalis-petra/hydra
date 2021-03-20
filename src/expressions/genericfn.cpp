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

// GENERIC FUNCTION

GenericFn::GenericFn() {
  is_fn = true;
  type->rest_type = nullptr;
  type->return_type = new type::Any;
}

string GenericFn::to_string(LocalRuntime &r, LexicalScope &s) {
  return "<generic function>";
}

void GenericFn::mark_node() {
  if (marked)
    return;

  marked = true;
  type->mark_node();
  docstring->mark_node();
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
  // now we have a sorted list, we just need to execute the methods, one-by-one
  // only executing the next method if the previous one executed
  // in addition, we need to introduce the 'call-next' function into
  // the lexical scope
  if (applicables.empty()) {
    string err = "no applicable method found in generic function";
    throw err;
  } else {
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
      roots.remove(obj);
    }
    if (uop) {
      uop->scope->map.erase(call_next);
    }
    return out;
  }
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
