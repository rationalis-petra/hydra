#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <typeinfo>

#include "expressions.hpp"
#include "operations.hpp"

using std::list;
using std::map;
using std::string;
using std::function;

hydra_oper::hydra_oper() {
  type = new type_fn;
  type->return_type = new type_nil;
  type->rest_type = nullptr;
}

list<hydra_object *> make_list(hydra_object *obj) {
  list<hydra_object *> lst;
  while (!obj->null()) {
    hydra_cons *cns = hydra_cast<hydra_cons>(obj);
    lst.push_back(cns->car);
    obj = cns->cdr;
  }
  return lst;
}

user_oper::user_oper(hydra_object *op_def, bool _is_fn, runtime &r,
                     lexical_scope &s) : hydra_oper() {
  is_fn = _is_fn;
  rest = nullptr;
  self = nullptr;
  type->rest_type = nullptr;
  docstring = new hydra_string;
  scope = new lexical_scope(s);

  // operations have the form:
  // (fn/mac <optional name> <arg_list> rest)
  if (hydra_cons *cns = dynamic_cast<hydra_cons *>(op_def)) {
    hydra_object *name_list = cns->car;

    list<hydra_object *> lst = make_list(name_list);
    bool optional = false;
    bool key = false;

    for (auto it = lst.begin(); it != lst.end(); it++) {
      hydra_symbol* name;
      hydra_type* t_type;
      if (hydra_cons* symdef = dynamic_cast<hydra_cons*>(*it)) {
        name = hydra_cast<hydra_symbol>(symdef->car);
        t_type = type_from_rep(hydra_cast<hydra_cons>(symdef->cdr)->car);
      } else {
        name = hydra_cast<hydra_symbol>(*it);
        t_type = new type_nil;
      }
      // :self keyword
      if (name == hydra_cast<hydra_module>(r.root->intern("keyword")->value)
                      ->get("self")) {
        if (++it != lst.end()) {
          self = hydra_cast<hydra_symbol>(*it);
          //type->return_type = t_type;
        } else {
          string err = "No self argument name!";
          throw err;
        }
      }

      // :rest arguments
      else if (name ==
               hydra_cast<hydra_module>(r.root->intern("keyword")->value)
                   ->get("rest")) {
        if (++it != lst.end()) {
          rest = hydra_cast<hydra_symbol>(*it);
          type->rest_type = new type_nil;
        } else {
          string err = "No rest argument name provided!";
          throw err;
        }
      }

      // :optional arguments
      else if (name ==
               hydra_cast<hydra_module>(r.root->intern("keyword")->value)
                   ->get("optional")) {
        if (key) {
          string err = "Cannot follow :key with :optional in function list";
          throw err;
        } else {
          optional = true;
        }
      }

      else if (name ==
               hydra_cast<hydra_module>(r.root->intern("keyword")->value)
                   ->get("key")) {
        key = true;
      }

      // :key arguments
      else {
        if (key) {
          hydra_symbol *keyword =
              hydra_cast<hydra_module>(r.root->intern("keyword")->value)
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
    hydra_object *expr_body = cns->cdr;
    hydra_object *cdr = expr_body;

    if (hydra_cons *docons = (dynamic_cast<hydra_cons *>(expr_body))) {
      if (hydra_string *dostring = dynamic_cast<hydra_string *>(docons->car)) {
        docstring = dostring;
      }
    }

    hydra_symbol *progn = hydra_cast<hydra_symbol>(
        hydra_cast<hydra_module>(
            hydra_cast<hydra_symbol>(language_module->get("core"))->value)
            ->get("progn"));
    progn->name = "progn";
    hydra_object *car = progn;
    expr = new hydra_cons(car, cdr);
  } else {
    string err = "Non-cons provided to fn/mac";
    throw err;
  }
}

hydra_object *user_oper::call(hydra_object *alist, runtime &r,
                              lexical_scope &s) {
  // if this is a macro or function, argument evaluation (or lack thereof) is
  // taken care of
  list<hydra_object *> arg_list = get_arg_list(alist, r, s);

  // too few arguments OR too many arguments
  if ((arg_list.size() < arg_names.size()) ||
      (((arg_list.size() > arg_names.size() + optionals.size() + (keys.size() * 2)) &&
        rest == nullptr))) {
    string err = "Error: operation called with incorrect arg_count!";
    throw err;
  }

  // so, all we need to to is perform lexical substitution!
  for (hydra_symbol *s : arg_names) {
    scope->map[s] = arg_list.front();
    arg_list.pop_front();
  }
  for (hydra_symbol *s : optionals) {
    if (arg_list.empty()) {
      scope->map[s] = hydra_nil::get();
    } else {
      scope->map[s] = arg_list.front();
      arg_list.pop_front();
    }
  }

  // set all keys to nil
  for (auto s : keys) {
    scope->map[s.second] = hydra_nil::get();
  }

  int keycounter = keys.size();
  while (keycounter--) {
    if (!arg_list.empty()) {
      hydra_symbol *sym = hydra_cast<hydra_symbol>(arg_list.front());
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
      scope->map[rest] = hydra_nil::get();
    } else {
      // we use a recursive lambda to construct the rlist
      function<hydra_object *()> gen_rest = [&](void) {
        if (arg_list.empty()) {
          return (hydra_object *)hydra_nil::get();
        } else {
          hydra_object *car = arg_list.front();
          arg_list.pop_front();
          hydra_object *cdr = gen_rest();
          return (hydra_object *) new hydra_cons(car, cdr);
        }
      };
      scope->map[rest] = gen_rest();
    }
  }
  if (self) {
    scope->map[self] = this;
  }
  hydra_object *out;
  if (is_fn) {
    out = expr->eval(r, *scope);
  } else {
    out = expr->eval(r, *scope)->eval(r, s);
  }
  hydra_object::roots.insert(out);
  hydra_object::collect_garbage(r);
  hydra_object::roots.erase(out);
  return out;
}

user_oper::~user_oper() {
  delete scope;
}

void combined_fn::add(hydra_oper* fn) {
  if (combined_fn *f = dynamic_cast<combined_fn*>(fn)) {
    for (hydra_oper* o : f->functions) {
      add(o);
    }
  } else {
    functions.push_front(fn);
  }
}

hydra_object* combined_fn::call(hydra_object* alist, runtime &r, lexical_scope &s) {
  list<hydra_object*> arg_list = get_arg_list(alist, r, s);

  if (type->check_args(arg_list)->null()) {
    string err = "Type error calling function";
    throw err;
  }
  for (hydra_oper* fn : functions) {
    if (!fn->type->check_args(arg_list)->null()) {
      return fn->call(alist, r, s);
    }
  }
  string err = "No matching function in combined function";
  throw err;
}

list<hydra_object *> hydra_oper::get_arg_list(hydra_object *arg_list,
                                              runtime &r, lexical_scope &s) {
  list<hydra_object *> out_list;
  while (!arg_list->null()) {
    hydra_cons *list_elt = dynamic_cast<hydra_cons *>(arg_list);
    hydra_object *arg = list_elt->car;
    if (is_fn) {
      out_list.push_back(arg->eval(r, s));
    } else {
      out_list.push_back(arg);
    }
    arg_list = list_elt->cdr;
  }
  if (type->check_args(out_list)->null()) {
    string err = "type check failed!";
    throw err;
  }
  return out_list;
}

string combined_fn::to_string() const { return "<combined function>"; }

string user_oper::to_string() const { return "<user-defined operation>"; }

string hydra_oper::to_string() const { return "<inbuilt operation>"; }

