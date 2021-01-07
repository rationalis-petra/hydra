#include <map>
#include <string>
#include <typeinfo>

#include "expressions.hpp"

using namespace std;

ivy_object *lexical_subst(ivy_object *expr, map<string, ivy_object *> subst_map,
                          bool quote_args);

user_oper::user_oper(ivy_object *op_def, bool _eval_args) {
  eval_args = _eval_args;

  // operations have the form:
  // (fn/mac <optional name> <arg_list> rest)
  if (ivy_cons *cns = dynamic_cast<ivy_cons *>(op_def)) {
    ivy_object *name_list = cns->car;

    while (!name_list->null()) {
      if (ivy_cons *name_elt = dynamic_cast<ivy_cons *>(name_list)) {
        if (ivy_symbol *name = dynamic_cast<ivy_symbol *>(name_elt->car)) {
          arg_names.push_back(name->symbol);
          name_list = name_elt->cdr;
        } else {
          string err = "non-name provided to fn";
          throw err;
        }
      } else {
        string err = "list parsing error in user_oper!";
        throw err;
      }
    }
    ivy_object *expr_body = cns->cdr;
    ivy_cons *texpr = new ivy_cons();
    texpr->cdr = expr_body;
    ivy_symbol *progn = new ivy_symbol();
    progn->symbol = "progn";
    texpr->car = progn;
    expr = texpr;
  } else {
    string err = "Non-cons provided to fn/mac";
    throw err;
  }
}

ivy_object *user_oper::call(ivy_object *alist, runtime &r) {
  // if eval_args is true, it will be taken care of
  list<ivy_object *> arg_list = get_arg_list(alist, r);

  if (arg_list.size() != arg_names.size()) {
    string err = "Error: function called with incorrect arg_count!";
    throw err;
  }

  // so, all we need to to is perform lexical substitution!
  map<string, ivy_object *> subst_map;
  for (string s : arg_names) {
    subst_map[s] = arg_list.front();
    arg_list.pop_front();
  }
  return lexical_subst(expr, subst_map, eval_args)->eval(r);
}

ivy_object *lexical_subst(ivy_object *expr, map<string, ivy_object *> subst_map,
                          bool quote_args) {
  // we want to assert that len(arg_list) = len(vals)
  // we can now assume that conslen(arg_list) = conslen(vals)
  if (ivy_symbol *sym = dynamic_cast<ivy_symbol *>(expr)) {
    if (subst_map.find(sym->symbol) != subst_map.end()) {
      if (quote_args) {
        ivy_symbol *qsym = new ivy_symbol;
        qsym->symbol = "quote";

        ivy_cons *quoted_arg = new ivy_cons();
        quoted_arg->car = subst_map.at(sym->symbol);
        quoted_arg->cdr = new ivy_nil();

        ivy_cons *quote = new ivy_cons();
        quote->car = qsym;
        quote->cdr = quoted_arg;
        return quote;
      } else {
        return subst_map.at(sym->symbol);
      }
    }
    return expr;
  } else if (ivy_cons *cns = dynamic_cast<ivy_cons *>(expr)) {
    ivy_cons *out = new ivy_cons();
    out->car = lexical_subst(cns->car, subst_map, quote_args);
    out->cdr = lexical_subst(cns->cdr, subst_map, quote_args);
    return out;
  } else {
    return expr;
  }
}

list<ivy_object *> ivy_oper::get_arg_list(ivy_object *arg_list, runtime &r) {
  list<ivy_object *> out_list;
  while (!arg_list->null()) {
    ivy_cons *list_elt = dynamic_cast<ivy_cons *>(arg_list);
    ivy_object *arg = list_elt->car;
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

string ivy_oper::to_string() const { return "inbuilt operation"; }
