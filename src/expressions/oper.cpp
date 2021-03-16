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
string Operator::to_string() const { return "<inbuilt operation>"; }

Operator::Operator() {
  type = new type::Fn;
  docstring = new HString;
  invoker = this;
}

void Operator::mark_node() {
  if (marked) return;
  Object::mark_node();
  docstring->mark_node();
  type->mark_node();
}

list<Object *> Operator::get_arg_list(Object *arg_list, LocalRuntime &r,
                                     LexicalScope &s) {
  // ASSUME that arg_list is rooted
  Object *original_list = arg_list;

  list<Object*> alist = cons_to_list(arg_list);
  list<Object *> out_list;
  if (is_fn) {
    for (Object* arg : alist) {
      out_list.push_back(arg->eval(r, s));
      Object::roots.insert(out_list.back());
    }
  } else {
    out_list = alist;
    // TODO: make me able to remove this!!
    for (Object* arg : alist) {
      Object::roots.insert(arg);
    }
  }
  if (type->check_args(out_list)->null()) {
    string err =
        "type check failed! for arg_list: " + original_list->to_string() +
        "expected type: " + type->to_string();
    throw err;
  }
  return out_list;
}

// GENERIC FUNCTION

string GenericFn::to_string() const { return "<generic function>"; }

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

Object *GenericFn::call(list<Object*> arg_list, LocalRuntime &r, LexicalScope &s) {
  // ASSUME that the function itself is marked

  // first find a list of applicable functions
  std::list<Operator*> applicables;
  for (Operator *fn : functions) {
    if (!fn->type->check_args(arg_list)->null()) {
      applicables.push_back(fn);

      // we need to unroot values in the arg_list
    }
  }

  // Now, we have a list of applicable functions, we need to support them
  // using the subtype relation
  applicables.sort([](Operator *op1, Operator *op2) {
    return (op1->type->subtype(op2->type))->null();
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
    if (UserOperator* uop = dynamic_cast<UserOperator*>(op)) {
      // TODO come up with a better solution than permanelty changing call-next...
      uop->scope->map[call_next] = nextfnc;
    }
    return op->call(arg_list, r, s);
  }
}

InbuiltOperator::InbuiltOperator(string _docstring,
                                 Object *(*_fnc)(std::list<Object *> arg_list,
                                                LocalRuntime &r,
                                                LexicalScope &s),
                                 type::Fn *t, bool isfn) {
  fnc = _fnc;
  docstring = new HString(_docstring);
  type = t;
  is_fn = isfn;
}

Object *InbuiltOperator::call(std::list<Object *> arg_list, LocalRuntime &r,
                             LexicalScope &s) {
  // ASSUME arg_list is rooted
  // WE delegate rooting to inbuilt functions
  return fnc(arg_list, r, s);
}

NextFnc::NextFnc(list<Operator*> _fncs, list<Object*> _arg_list, Symbol* _nextsym) {
  funcs = _fncs;
  arg_list = _arg_list;
  nextsym = _nextsym;
}

Object* NextFnc::call(std::list<Object*> arglist, LocalRuntime& r, LexicalScope& s) {
  for (Object* obj : arglist) {
    roots.remove(obj);
  }
  Operator* op = funcs.front();
  funcs.pop_front();
  s.map[nextsym] = this;
  return op->call(arg_list, r, s);
}
