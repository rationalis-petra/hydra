#include <map>
#include <string>
#include <typeinfo>

#include "expressions.hpp"

using namespace std;

hydra_object *lexical_subst(hydra_object *expr,
                            map<string, hydra_object *> subst_map,
                            bool quote_args);

user_oper::user_oper(hydra_object *op_def, bool _eval_args) {
  eval_args = _eval_args;

  // operations have the form:
  // (fn/mac <optional name> <arg_list> rest)
  if (hydra_cons *cns = dynamic_cast<hydra_cons *>(op_def)) {
    hydra_object *name_list = cns->car;

    while (!name_list->null()) {
      if (hydra_cons *name_elt = dynamic_cast<hydra_cons *>(name_list)) {
        if (hydra_symbol *name = dynamic_cast<hydra_symbol *>(name_elt->car)) {
          if (name->symbol == ":rest") {
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
            rest = name->symbol;
            name_list = name_elt->cdr;
          }
          else if (name->symbol == ":self") {
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
            self = name->symbol;
            name_list = name_elt->cdr;
          }
          else {
            arg_names.push_back(name->symbol);
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
    hydra_symbol *progn = new hydra_symbol();
    progn->symbol = "progn";
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

  if ((arg_list.size() != arg_names.size()) && rest == "") {
    string err = "Error: function called with incorrect arg_count!";
    throw err;
  }

  // so, all we need to to is perform lexical substitution!
  map<string, hydra_object *> subst_map;
  for (string s : arg_names) {
    subst_map[s] = arg_list.front();
    arg_list.pop_front();
  }
  if (rest != "") {
    // generate a list containing the rest of the arg_list
    // if list is empty, use nil!
    if (arg_list.empty()) {
      subst_map[rest] = new hydra_nil;
    }
    else {
      hydra_cons* rlist = new hydra_cons;
      hydra_cons* root = rlist;
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
  if (self != "") {
    subst_map[self] = this;
  }
  return lexical_subst(expr, subst_map, eval_args)->eval(r);
}

hydra_object *lexical_subst(hydra_object *expr,
                            map<string, hydra_object *> subst_map,
                            bool quote_args) {
  // we want to assert that len(arg_list) = len(vals)
  // we can now assume that conslen(arg_list) = conslen(vals)
  if (hydra_symbol *sym = dynamic_cast<hydra_symbol *>(expr)) {
    if (subst_map.find(sym->symbol) != subst_map.end()) {
      if (quote_args) {
        hydra_symbol *qsym = new hydra_symbol;
        qsym->symbol = "quote";

        hydra_cons *quoted_arg = new hydra_cons();
        quoted_arg->car = subst_map.at(sym->symbol);
        quoted_arg->cdr = new hydra_nil();

        hydra_cons *quote = new hydra_cons();
        quote->car = qsym;
        quote->cdr = quoted_arg;
        return quote;
      } else {
        return subst_map.at(sym->symbol);
      }
    }
    return expr;
  } else if (hydra_cons *cns = dynamic_cast<hydra_cons *>(expr)) {
    hydra_cons *out = new hydra_cons();
    out->car = lexical_subst(cns->car, subst_map, quote_args);
    out->cdr = lexical_subst(cns->cdr, subst_map, quote_args);
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
