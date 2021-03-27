#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <typeinfo>

#include "expressions.hpp"
#include "operations.hpp"
#include "utils.hpp"

using namespace expr;
using namespace interp;

using type::hydra_cast;

using std::list;
using std::string;
using std::function;

string UserOperator::to_string(LocalRuntime &r, LexicalScope &s) {
  string type = is_fn ? "function" : "macro";
  if (self) {
    return "<" + type + ": " + self->name + ">";
  } else {
    return "<anonymous " + type + ">";
  }
}

void UserOperator::mark_node() {
  if (marked) return;
  Object::mark_node();
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

UserOperator::UserOperator(std::list<Object *> op_def, bool _is_fn,
                           LocalRuntime &r, LexicalScope &s)
    : Operator() {

  // EVAL is called when evaluating
  // therefore, we must add ourselves to the root list
  // ASSUME op_def already there
  collector->insert_root(this);
  is_fn = _is_fn;
  rest = nullptr;
  self = nullptr;
  type->rest_type = nullptr;
  scope = new LexicalScope(s);

  // if we have a symbol, try evaluating it to see if it's a type

  // operations have the form:
  // (fn/mac <optional type> <lambda_list> rest)

  // lambd list looks like: (x y (z Integer) :rest p)
  list<Object*> lambda_list;

  // STEP 1: determine if this function is given a return type!
  if (Symbol *sym = dynamic_cast<Symbol *>(op_def.front())) {
    // The first element is a symbol, so we evaluate to get a type!
    Object *raw_type = sym->eval(r, s);
    if (type::Type *ret_type = dynamic_cast<type::Type *>(raw_type)) {
      type->return_type = ret_type;
    } else {
      string err = "First argument to fn is neither type nor argument-list";
      throw err;
    }

    // NOW, we need to make sure that the /second/ argument is a lambda-list
    op_def.pop_front();
    if (Cons *cns = dynamic_cast<Cons *>(op_def.front())) {
      lambda_list = cons_to_list(cns);
    } else {
      string err = "Second argument to typed fn is not an argument-list";
      throw err;
    }
  } else {
    lambda_list = cons_to_list(op_def.front());
  }

  bool optional = false;
  bool key = false;

  for (auto it = lambda_list.begin(); it != lambda_list.end(); it++) {
    Symbol *name;
    type::Type *t_type;
    if (Cons *symdef = dynamic_cast<Cons *>(*it)) {
      name = hydra_cast<Symbol>(symdef->car);
      // type
      t_type = hydra_cast<type::Type>(
          hydra_cast<Cons>(symdef->cdr)->car->eval(r, s));
    } else {
      name = hydra_cast<Symbol>(*it);
      t_type = new type::Any;
    }
    // :self keyword
    if (name == get_keyword("self")) {
      if (++it != lambda_list.end()) {
        self = hydra_cast<Symbol>(*it);
        // type->return_type = t_type;
      } else {
        string err = "No self argument name!";
        throw err;
      }
    }

    // :rest arguments
    else if (name == get_keyword("rest")) {
      if (++it != lambda_list.end()) {
        if (Cons *cns = dynamic_cast<Cons *>(*it)) {
          rest = hydra_cast<Symbol>(cns->car);
          type->rest_type = hydra_cast<type::Type>(
              hydra_cast<Cons>(cns->cdr)->car->eval(r, s));
        } else {
          rest = hydra_cast<Symbol>(*it);
          type->rest_type = new type::Any;
        }
      } else {
        string err = "No rest argument name provided!";
        throw err;
      }
    }

    // :optional arguments
    else if (name == get_keyword("optional")) {
      if (key) {
        string err = "Cannot follow :key with :optional in function list";
        throw err;
      } else {
        optional = true;
      }
    }

    else if (name == get_keyword("key")) {
      key = true;
    }

    // :key arguments
    else {
      if (key) {
        Symbol *keyword = get_keyword(name->name);
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

  
  // Now, check for a docstring
  op_def.pop_front();
  if (HString *dstring = dynamic_cast<HString *>(op_def.front())) {
    docstring = dstring;
    if (op_def.size() != 1) {
      op_def.pop_front();
    }
  }

  Symbol *progn = hydra_cast<Symbol>(
      hydra_cast<Module>(
          hydra_cast<Symbol>(language_module->get("core"))->value)
          ->get("progn"));
  progn->name = "progn";
  Object *car = progn;
  Object* cdr = list_to_cons(op_def);
  expr = new Cons(car, cdr);

  // see beginning of function
  collector->remove_root(this);
}

Object *UserOperator::call(list<Object*> arg_list, LocalRuntime &r,
                           LexicalScope &s, bool macexpand) {
  // ASSUME that this and the alist are rooted
  // ASSUME all values in arg_list are rooted

  // clone the scope into s

  // too few arguments OR too many arguments
  if ((arg_list.size() < arg_names.size()) ||
      (((arg_list.size() > arg_names.size() + optionals.size() + (keys.size() * 2)) &&
        rest == nullptr))) {
    string err = "Error: operation called with incorrect arg_count!";
    throw err;
  }

  LexicalScope* scope = new LexicalScope(*this->scope);

  // so, all we need to to is perform lexical substitution!
  for (Symbol *sym : arg_names) {
    scope->map[sym] = arg_list.front();
    // unroot values as we add them to a scope
    collector->remove_root(arg_list.front());
    arg_list.pop_front();
  }
  for (Symbol *s : optionals) {
    if (arg_list.empty()) {
      scope->map[s] = nil::get();
    } else {
      scope->map[s] = arg_list.front();
      collector->remove_root(arg_list.front());
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
          collector->remove_root(arg_list.front());
          arg_list.pop_front();
        }
      } else {
        string err = "Invalid keyword provided";
        throw err;
      }
      // we are done with sym, unroot it
      collector->remove_root(sym);
    }
  }

  if (rest) {
    // generate a list containing the rest of the arg_list
    // if list is empty, use nil!
    if (arg_list.empty()) {
      scope->map[rest] = nil::get();
    } else {
      // we use a recursive lambda to construct the rlist
      function<Object *()> gen_rest = [&](void) {
        if (arg_list.empty()) {
          return (Object *)nil::get();
        } else {
          Object *car = arg_list.front();
          collector->remove_root(arg_list.front());
          arg_list.pop_front();
          Object *cdr = gen_rest();
          return (Object *) new Cons(car, cdr);
        }
      };
      scope->map[rest] = gen_rest();
    }
  }
  if (self) {
    scope->map[self] = this;
  }

  Object *out;
  if (is_fn) {
    out = expr->eval(r, *scope);
  } else {
    Object* intermediate = expr->eval(r, *scope);
    if (!macexpand) {
      collector->insert_root(intermediate);
      out = intermediate->eval(r, s);
      collector->remove_root(intermediate);
    } else {
      out = intermediate;
    }
  }
  delete scope;

  return out;
}

UserOperator::~UserOperator() {
  delete scope;
}
