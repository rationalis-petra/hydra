#include <map>
#include <string>
#include <typeinfo>

#include "expressions.hpp"

using namespace std;

hydra_object *lexical_subst(hydra_object *expr,
                            map<hydra_symbol *, hydra_object *> subst_map);

user_oper::user_oper(hydra_object *op_def, bool _eval_args, runtime& r) {
  eval_args = _eval_args;
  rest = nullptr;
  self = nullptr;

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

hydra_object *user_oper::call(hydra_object *alist, runtime &r) {
  // if eval_args is true, it will be taken care of
  list<hydra_object *> arg_list = get_arg_list(alist, r);

  if ((arg_list.size() != arg_names.size()) && rest == nullptr) {
    string err = "Error: function called with incorrect arg_count!";
    throw err;
  }

  // so, all we need to to is perform lexical substitution!
  map<hydra_symbol *, hydra_object *> subst_map;
  for (hydra_symbol *s : arg_names) {
    subst_map[s] = arg_list.front();
    arg_list.pop_front();
  }
  if (rest) {
    // generate a list containing the rest of the arg_list
    // if list is empty, use nil!
    if (arg_list.empty()) {
      subst_map[rest] = new hydra_nil;
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
      subst_map[rest] = root;
    }
  }
  if (self) {
    subst_map[self] = this;
  }
  if (eval_args) {
    return lexical_subst(expr, subst_map)->eval(r);
  } else {
    return lexical_subst(expr, subst_map)->eval(r)->eval(r);
  }
}

hydra_object *lexical_subst(hydra_object *expr,
                            map<hydra_symbol *, hydra_object *> subst_map) {
  // we want to assert that len(arg_list) = len(vals)
  // we can now assume that conslen(arg_list) = conslen(vals)
  if (hydra_symbol *sym = dynamic_cast<hydra_symbol *>(expr)) {
    if (subst_map.find(sym) != subst_map.end()) {
      if (true) {
        hydra_symbol *ssym = new hydra_symbol(sym->name);
        ssym->value = subst_map.at(sym);

        return ssym;
      } else {
        return subst_map.at(sym);
      }
    }
    return expr;
  } else if (hydra_cons *cns = dynamic_cast<hydra_cons *>(expr)) {
    hydra_cons *out = new hydra_cons();
    out->car = lexical_subst(cns->car, subst_map);

    // don't substitute quoted arguments
    if (hydra_symbol *qte = dynamic_cast<hydra_symbol *>(out->car)) {
      if (qte->name == "quote") {
        out->cdr = cns->cdr;
      } else {
        out->cdr = lexical_subst(cns->cdr, subst_map);
      }
    } else {
      out->cdr = lexical_subst(cns->cdr, subst_map);
    }

    return out;
  } else {
    return expr;
  }
}

list<hydra_object *> hydra_oper::get_arg_list(hydra_object *arg_list,
                                              runtime &r) {
  list<hydra_object *> out_list;
  while (!arg_list->null()) {
    hydra_cons *list_elt = dynamic_cast<hydra_cons *>(arg_list);
    hydra_object *arg = list_elt->car;
    if (eval_args) {
      out_list.push_back(arg->eval(r));
    } else {
      out_list.push_back(arg);
    }
    arg_list = list_elt->cdr;
  }
  return out_list;
}

string user_oper::to_string() const { return "user-defined operation"; }

string hydra_oper::to_string() const { return "inbuilt operation"; }
