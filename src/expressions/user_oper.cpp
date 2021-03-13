#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <typeinfo>

#include "expressions.hpp"
#include "operations.hpp"

using namespace expr;

using type::hydra_cast;

using std::list;
using std::map;
using std::string;
using std::function;

// UTILITY FUNCTION: local to this file
list<Value *> make_list(Value *obj) {
  list<Value *> lst;
  while (!obj->null()) {
    Cons *cns = hydra_cast<Cons>(obj);
    lst.push_back(cns->car);
    obj = cns->cdr;
  }
  return lst;
}

string UserOperator::to_string() const {
  string name = is_fn ? "function" : "macro";
  if (self) {
    return "<" + name + ": " + self->to_string() + ">";
  } else {
    return "<user-defined operation>";
  }
}

void UserOperator::mark_node() {
  if (marked) return;
  marked = true;
  docstring->mark_node();
  type->mark_node();

  if (rest) {
    rest->mark_node();
  }

  if (self) self->mark_node();
  for (Symbol* n : arg_names) {
    n->mark_node();
  }
  for (Symbol * s : optionals) {
    s->mark_node();
  }
  for (auto pair : keys) {
    pair.first->mark_node();
    pair.second->mark_node();
  }

  expr->mark_node();

  for (auto pair : scope->map) {
    pair.first->mark_node();
    pair.second->mark_node();
  }
}

UserOperator::UserOperator(Value *op_def, bool _is_fn, LocalRuntime &r,
                     LexicalScope &s) : Operator() {
  is_fn = _is_fn;
  rest = nullptr;
  self = nullptr;
  type->rest_type = nullptr;
  scope = new LexicalScope(s);

  // if we have a symbol, try evaluating it to see if it's a type

  // operations have the form:
  // (fn/mac <optional name> <arg_list> rest)
  if (Cons *cns = dynamic_cast<Cons *>(op_def)) {

    Value *name_list;

    if (Symbol *sym = dynamic_cast<Symbol *>(cns->car)) {
      Value *raw_type = sym->eval(r, s);
      if (type::Type* ret_type = dynamic_cast<type::Type*>(raw_type)) {
        type->return_type = ret_type;
      } else {
        string err = "First argument to fn is neither type nor argument-list";
        throw err;
      }
      if (Cons* cns2 = dynamic_cast<Cons *>(cns->cdr)) {
        name_list = cns2->car;
        cns = cns2;
      } else {
        string err = "Second argument to typed fn is not an argument-list";
        throw err;
      }
    } else {
      name_list = cns->car;
    }


    list<Value *> lst = make_list(name_list);
    bool optional = false;
    bool key = false;

    for (auto it = lst.begin(); it != lst.end(); it++) {
      Symbol* name;
      type::Type* t_type;
      if (Cons* symdef = dynamic_cast<Cons*>(*it)) {
        name = hydra_cast<Symbol>(symdef->car);
        t_type = hydra_cast<type::Type>(hydra_cast<Cons>(symdef->cdr)->car->eval(r, s));
      } else {
        name = hydra_cast<Symbol>(*it);
        t_type = new type::Any;
      }
      // :self keyword
      if (name == hydra_cast<Module>(r.r.root->intern("keyword")->value)
                      ->get("self")) {
        if (++it != lst.end()) {
          self = hydra_cast<Symbol>(*it);
          //type->return_type = t_type;
        } else {
          string err = "No self argument name!";
          throw err;
        }
      }

      // :rest arguments
      else if (name ==
               hydra_cast<Module>(r.r.root->intern("keyword")->value)
                   ->get("rest")) {
        if (++it != lst.end()) {
          if (Cons* cns = dynamic_cast<Cons*>(*it)) {
            rest = hydra_cast<Symbol>(cns->car);
            type->rest_type = hydra_cast<type::Type>(hydra_cast<Cons>(cns->cdr)->car->eval(r, s));
          }
          else {
            rest = hydra_cast<Symbol>(*it);
            type->rest_type = new type::Any;
          }
        } else {
          string err = "No rest argument name provided!";
          throw err;
        }
      }

      // :optional arguments
      else if (name ==
               hydra_cast<Module>(r.r.root->intern("keyword")->value)
                   ->get("optional")) {
        if (key) {
          string err = "Cannot follow :key with :optional in function list";
          throw err;
        } else {
          optional = true;
        }
      }

      else if (name ==
               hydra_cast<Module>(r.r.root->intern("keyword")->value)
                   ->get("key")) {
        key = true;
      }

      // :key arguments
      else {
        if (key) {
          Symbol *keyword =
              hydra_cast<Module>(r.r.root->intern("keyword")->value)
                  ->intern(name->name);
          keys[keyword] = name;
          keyword->value = keyword;
          type->keyword_list.push_back(t_type);
          type->keyword_names.push_back(name);

        } else if (optional) {
          optionals.push_back(name);
          type->optional_list.push_back(t_type);

        } else {
          type->arg_list.push_back(t_type);
          arg_names.push_back(name);

        }
      }
    }
    Value *expr_body = cns->cdr;
    Value *cdr = expr_body;

    if (Cons *docons = (dynamic_cast<Cons *>(expr_body))) {
      HString *dstring = dynamic_cast<HString *>(docons->car);
      if (dstring) {
        docstring = dstring;
      }
    }

    Symbol *progn = hydra_cast<Symbol>(
        hydra_cast<Module>(
            hydra_cast<Symbol>(language_module->get("core"))->value)
            ->get("progn"));
    progn->name = "progn";
    Value *car = progn;
    expr = new Cons(car, cdr);
  } else {
    string err = "Non-cons provided to fn/mac";
    throw err;
  }
}

Value *UserOperator::call(Value *alist, LocalRuntime &r,
                              LexicalScope &s) {
  // if this is a macro or function, argument evaluation (or lack thereof) is
  // taken care of by get_arg_list
  list<Value *> arg_list = get_arg_list(alist, r, s);

  // too few arguments OR too many arguments
  if ((arg_list.size() < arg_names.size()) ||
      (((arg_list.size() > arg_names.size() + optionals.size() + (keys.size() * 2)) &&
        rest == nullptr))) {
    string err = "Error: operation called with incorrect arg_count!";
    throw err;
  }

  // so, all we need to to is perform lexical substitution!
  for (Symbol *s : arg_names) {
    scope->map[s] = arg_list.front();
    arg_list.pop_front();
  }
  for (Symbol *s : optionals) {
    if (arg_list.empty()) {
      scope->map[s] = nil::get();
    } else {
      scope->map[s] = arg_list.front();
      arg_list.pop_front();
    }
  }

  // set all keys to nil
  for (auto s : keys) {
    scope->map[s.second] = nil::get();
  }

  int keycounter = keys.size();
  while (keycounter--) {
    if (!arg_list.empty()) {
      Symbol *sym = hydra_cast<Symbol>(arg_list.front());
      arg_list.pop_front();
      if (keys.find(sym) != keys.end()) {
        if (arg_list.empty()) {
          string err = "Uneven number of keyword arguments";
          throw err;
        } else {
          scope->map[keys[sym]] = arg_list.front();
          arg_list.pop_front();
        }
      } else {
        string err = "Invalid keyword provided";
        throw err;
      }
    }
  }
  if (rest) {
    // generate a list containing the rest of the arg_list
    // if list is empty, use nil!
    if (arg_list.empty()) {
      scope->map[rest] = nil::get();
    } else {
      // we use a recursive lambda to construct the rlist
      function<Value *()> gen_rest = [&](void) {
        if (arg_list.empty()) {
          return (Value *)nil::get();
        } else {
          Value *car = arg_list.front();
          arg_list.pop_front();
          Value *cdr = gen_rest();
          return (Value *) new Cons(car, cdr);
        }
      };
      scope->map[rest] = gen_rest();
    }
  }
  if (self) {
    scope->map[self] = this;
  }


  Value *out;
  if (is_fn) {
    out = expr->eval(r, *scope);
  } else {
    out = expr->eval(r, *scope)->eval(r, s);
  }

  return out;
}

UserOperator::~UserOperator() {
  delete scope;
}
