#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <variant>
#include <fstream>
#include <utility>

#include "expressions.hpp"
#include "operations.hpp"
using namespace std;

extern string lang;

hydra_module* language_module;
vector<pair<string, hydra_object*>> inbuilts;

vector<pair<string, hydra_object*>> dev;
vector<pair<string, hydra_object*>> core;
vector<pair<string, hydra_object*>> foreign;
vector<pair<string, hydra_object*>> io;
// net
// concurrent / parallel

void make_modules();

int main(int argc, char **argv) {
  make_modules();

  runtime r;
  //hydra_object::r = &r;


  r.root = new hydra_module("");
  hydra_symbol* sym = r.root->intern("hydra");
  sym->value = language_module;
  r.active_module = language_module;
  sym = r.root->intern("keyword");
  sym->value = new hydra_module("keyword");
  // arithmetic


  hydra_module *mod = language_module;
  for (auto p : inbuilts) {
    hydra_symbol* sym = mod->intern(p.first);
    sym->value = p.second;
  }

  vector<pair<string, vector<pair<string, hydra_object *>>>> moddefs = {
      make_pair("core", core), make_pair("io", io),
      make_pair("foreign", foreign), make_pair("dev", dev)};

  for (auto m : moddefs) {
    mod = hydra_cast<hydra_module>(
        hydra_cast<hydra_symbol>(language_module->get(m.first))->value);
    for (auto p : m.second) {
      hydra_symbol *sym = mod->intern(p.first);
      mod->exported_symbols[p.first] = sym;
      sym->value = p.second;
    }
  }

  hydra_istream *stm = new hydra_istream();
  stm->stream = &cin;
  sym = hydra_cast<hydra_module>(language_module->intern("io")->value)->intern("+cin+");
  sym->value = stm;

  string in = lang;
  hydra_istream *prog = new hydra_istream();
  prog->stream = new stringstream(lang);
  hydra_object *ast;
  hydra_object *out;

  // if argc > 1, assume they are file names
  int count = argc;
  while (count > 0) {
    try {
      while (!prog->stream->eof()) {
        lexical_scope s;
        ast = read(prog, r);
        out = ast->eval(r, s);
      }
    } catch (string e) {
      cout << e << endl;
    } catch (const char *err) {
      cout << err << endl;
    }
    count--;
    if (count != 0) {
      delete prog->stream;
      prog->stream = new stringstream("(eval (load \"" + string(argv[argc - count]) + "\"))");
    }
  }

  while (true) {
    try {
      cout << r.active_module->name << "> ";
      lexical_scope s;
      ast = read(stm, r);
      hydra_object::roots.insert(ast);
      out = ast->eval(r, s);
      cout << "* " << out << endl;
      hydra_object::roots.remove(ast);
      hydra_object::collect_garbage(r);
    } catch (hydra_exception* e) {
      cout << "Top-level exception thrown!" << endl;
      cout << "Value: " << e->obj << endl;
    } catch (string e) {
      cout << e << endl;
    } catch (const char *err) {
      cout << err << endl;
    }

    getline(cin, in);
  }

  cout << hydra_object::node_list.size() << endl;
  for (hydra_object* obj : hydra_object::node_list) {
    cout << obj << endl;
    delete obj;
  }
  return 0;
}

void make_modules() {
  language_module = new hydra_module("hydra");
  inbuilts = {
      // other modules
      make_pair("core", new hydra_module("core")),
      make_pair("io", new hydra_module("io")),
      make_pair("foreign", new hydra_module("foreign")),
      make_pair("concurrent", new hydra_module("concurrent")),
      make_pair("dev", new hydra_module("dev"))
  };

  dev = {
      make_pair("doc", new op_describe)
  };

  combined_fn* gn_elt = new combined_fn;
  gn_elt->is_fn = true;
  gn_elt->type->rest_type = new type_nil;
  gn_elt->add(new op_vec_elt);
  gn_elt->add(new op_str_elt);
  gn_elt->add(new op_tuple_elt);

  
  combined_fn* gn_len = new combined_fn;
  gn_len->is_fn = true;
  gn_len->type->rest_type = new type_nil;
  gn_len->add(new op_vec_len);
  //gn_elt->add(new op_str_elt);


  combined_fn* gn_concat = new combined_fn;
  gn_concat->is_fn = true;
  gn_concat->type->rest_type = new type_nil;
  gn_concat->add(new op_str_cat);
  gn_concat->add(new op_vec_cat);

  combined_fn* gn_gr = new combined_fn;
  gn_gr->is_fn = true;
  gn_gr->type->arg_list.push_front(new type_nil);
  gn_gr->type->arg_list.push_front(new type_nil);

  gn_gr->add(new op_int_gr);
  gn_gr->add(new op_str_gr);

  combined_fn* gn_get = new combined_fn;
  gn_get->is_fn = true;
  gn_get->type->arg_list.push_front(new type_nil);
  gn_get->type->arg_list.push_front(new type_nil);
  gn_get->add(new op_obj_get);
  gn_get->add(new op_get);

  core = {

      // arithmetic
      make_pair("+", new op_plus), make_pair("-", new op_minus),
      make_pair("*", new op_multiply), make_pair("/", new op_divide),
      make_pair(">", gn_gr),
      // data
      make_pair("object", new op_object), 
      make_pair("tuple", new op_tuple),
      make_pair("union", new op_union),
      make_pair("vector", new op_vec),
      make_pair("cons", new op_cons),
      make_pair("car", new op_car),
      make_pair("cdr", new op_cdr),
      make_pair("elt", gn_elt),
      make_pair("concat", gn_concat),
      make_pair("len", gn_len),

      make_pair("get", gn_get),

      // streams
      make_pair("peek", new op_peek), make_pair("put", new op_put),
      make_pair("next", new op_next), make_pair("end?", new op_endp),
      make_pair("eval", new op_eval), make_pair("read", new op_read),
      make_pair("set-macro-character", new op_set_mac_char),
      // logic
      make_pair("=", new op_eq), make_pair("or", new op_or),
      make_pair("and", new op_and), make_pair("not", new op_not),

      // symbols

      make_pair("defined?", new op_defined), make_pair("set", new op_set),
      make_pair("unset", new op_unset),
      make_pair("lock", new op_lock), make_pair("unlock", new op_unlock),

      // language/control-flow
      make_pair("if", new op_if), make_pair("while", new op_while),
      make_pair("fn", new op_fn), make_pair("gen", new op_gen),
      make_pair("mac", new op_mac), make_pair("quote", new op_quote),
      make_pair("progn", new op_progn), make_pair("add-fn", new op_addfn),
      make_pair("exit", new op_exit),

      make_pair("module", new op_make_module),
      make_pair("symbol", new op_make_symbol),
      make_pair("in-module", new op_in_module),
      make_pair("current-module", new op_get_module),
      make_pair("get-symbols", new op_get_symbols),
      make_pair("export", new op_export),
      make_pair("insert", new op_insert),
      make_pair("intern", new op_intern),
      make_pair("remove", new op_remove),

      // CONDiTIONS
      make_pair("signal-condition", new op_signal_condition),
      make_pair("handler-case", new op_handler_catch),
      make_pair("handler-bind", new op_handler_bind),
      make_pair("with-restart", new op_add_restart),
      make_pair("get-restarts", new op_get_restarts),

      // types
      make_pair("Tuple", new type_tuple),
      make_pair("Integer", new type_integer),
      make_pair("Vector", new type_vector), make_pair("Type", new type_type),
      make_pair("String", new type_string),
      make_pair("Module", new type_module), make_pair("Nil", new type_nil),
      make_pair("Symbol", new type_symbol), make_pair("List", new type_list),
      make_pair("IOStream", new type_iostream),
      make_pair("IStream", new type_istream),
      make_pair("OStream", new type_ostream), make_pair("Fn", new type_fn),
      make_pair("Gen", new type_gen_fn), make_pair("type?", new op_typep),
      make_pair("type", new op_type)};

  foreign = {
      // ffi
      make_pair("load-c-library", new op_foreign_lib),
      make_pair("get-c-symbol", new op_foreign_sym),
      make_pair("internalize", new op_internalize)};

   io = {
      // io
      make_pair("print", new op_print),
      make_pair("open-file", new op_open_file),
      make_pair("close-file", new op_close)};
}
