#include <map>
#include <string>
#include <typeinfo>
#include <iostream>

#include "expressions.hpp"
#include "operations.hpp"

using namespace std;

list<hydra_object*> make_list(hydra_object* obj) {
  list<hydra_object*> lst;
  if (!obj->null()) {
    hydra_cons *cns = hydra_cast<hydra_cons>(obj);
    lst.push_back(cns->car);
    obj = cns->cdr;
  }
  return lst;
}


user_oper::user_oper(hydra_object *op_def, bool _is_fn, runtime& r, lexical_scope &s) {
  is_fn = _is_fn;
  rest = nullptr;
  self = nullptr;
  scope = new lexical_scope(s);

  // operations have the form:
  // (fn/mac <optional name> <arg_list> rest)
  if (hydra_cons *cns = dynamic_cast<hydra_cons *>(op_def)) {
    hydra_object *name_list = cns->car;

    // TODO: actually test for keyword!!!
    while (!name_list->null()) {
      if (hydra_cons *name_elt = dynamic_cast<hydra_cons *>(name_list)) {
        if (hydra_symbol *name = dynamic_cast<hydra_symbol *>(name_elt->car)) {
          if (name == hydra_cast<hydra_module>(r.root->intern("keyword")->value)
                          ->get("rest")) {
            name_elt = dynamic_cast<hydra_cons *>(name_elt->cdr);
            if (!name_elt) {
              string err = "No name provided to :rest!";
              throw err;
            }
            name = dynamic_cast<hydra_symbol *>(name_elt->car);
            if (!name) {
              string err = "non-symbol provided to fn/mac";
              throw err;
            }
            rest = name;
            name_list = name_elt->cdr;
          } else if (name ==
                     hydra_cast<hydra_module>(r.root->intern("keyword")->value)
                         ->get("self")) {
            name_elt = dynamic_cast<hydra_cons *>(name_elt->cdr);
            if (!name_elt) {
              string err = "No name provided to :self!";
              throw err;
            }
            name = dynamic_cast<hydra_symbol *>(name_elt->car);
            if (!name) {
              string err = "non-symbol provided to fn/mac";
              throw err;
            }
            self = name;
            name_list = name_elt->cdr;
          } else {
            arg_names.push_back(name);
            name_list = name_elt->cdr;
          }
        } else {
          string err = "non-name provided to fn";
          throw err;
        }
      } else {
        string err = "list parsing error in user_oper!";
        throw err;
      }
    }
    hydra_object *expr_body = cns->cdr;
    hydra_cons *texpr = new hydra_cons();
    texpr->cdr = expr_body;
    hydra_symbol *progn = hydra_cast<hydra_symbol>(
        hydra_cast<hydra_module>(
            hydra_cast<hydra_symbol>(language_module->get("core"))->value)
            ->get("progn"));
    progn->name = "progn";
    texpr->car = progn;
    expr = texpr;
  } else {
    string err = "Non-cons provided to fn/mac";
    throw err;
  }
}

hydra_object *user_oper::call(hydra_object *alist, runtime &r, lexical_scope& s) {
  // if eval_args is true, it will be taken care of
  list<hydra_object *> arg_list = get_arg_list(alist, r, s);

  if ((arg_list.size() != arg_names.size()) && rest == nullptr) {
    string err = "Error: function called with incorrect arg_count!";
    throw err;
  }

  // so, all we need to to is perform lexical substitution!
  for (hydra_symbol *s : arg_names) {
    scope->map[s] = arg_list.front();
    arg_list.pop_front();
  }
  if (rest) {
    // generate a list containing the rest of the arg_list
    // if list is empty, use nil!
    if (arg_list.empty()) {
      scope->map[rest] = new hydra_nil;
    } else {
      hydra_cons *rlist = new hydra_cons;
      hydra_cons *root = rlist;
      rlist->car = arg_list.front();
      arg_list.pop_front();
      while (!arg_list.empty()) {
        rlist->cdr = new hydra_cons;
        rlist = static_cast<hydra_cons *>(rlist->cdr);
        rlist->car = arg_list.front();
        arg_list.pop_front();
      }
      rlist->cdr = new hydra_nil;
      scope->map[rest] = root;
    }
  }
  if (self) {
    scope->map[self] = this;
  }
  if (is_fn) {
    return expr->eval(r, *scope);
  } else {
    return expr->eval(r, *scope)->eval(r, s);
  }
}

list<hydra_object *> hydra_oper::get_arg_list(hydra_object *arg_list,
                                              runtime &r, lexical_scope& s) {
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
  return out_list;
}

string user_oper::to_string() const { return "user-defined operation"; }

string hydra_oper::to_string() const { return "inbuilt operation"; }
