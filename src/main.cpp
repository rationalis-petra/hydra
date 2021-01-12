#include <iostream>
#include <string>
#include <map>
#include <variant>
#include <fstream>
#include <utility>

#include "expressions.hpp"
#include "operations.hpp"

using namespace std;

hydra_module* language_module;

int main(int argc, char **argv) {
  language_module = new hydra_module("hydra");
  pair<string, hydra_object*> inbuilts[] =
    {
      // arithmetic
     make_pair("+", new op_plus),
     make_pair("-", new op_minus),
     make_pair("*", new op_multiply),
     make_pair("/", new op_divide),
     make_pair(">=", new op_geq),
     // data
     make_pair("array", new op_arr),
     make_pair("cons", new op_cons),
     make_pair("car", new op_car),
     make_pair("cdr", new op_cdr),
     make_pair("elt", new op_elt),
     // io
     make_pair("print", new op_print),
     make_pair("open-file", new op_open_file),
     make_pair("close-file", new op_close),

     // streams
     make_pair("peek", new op_peek),
     make_pair("next", new op_next),
     make_pair("endp", new op_endp),
     make_pair("eval", new op_eval),
     make_pair("read", new op_read),
     make_pair("set-macro-character", new op_set_mac_char),
     // logic
     make_pair("=", new op_eq),
     make_pair("or", new op_or),
     make_pair("and", new op_and),
     make_pair("not", new op_not),
     // language
     make_pair("if", new op_if),
     make_pair("while", new op_while),
     make_pair("fn", new op_fn),
     make_pair("mac", new op_mac),
     make_pair("def", new op_def),
     make_pair("quote", new op_quote),
     make_pair("progn", new op_progn),
     make_pair("quit", new op_quit),

     make_pair("make-symbol", new op_in_module),
     make_pair("in-module", new op_in_module),
     make_pair("make-module", new op_make_module),
     make_pair("insert", new op_insert),
     make_pair("get", new op_get),
     //make_pair("remove", new op_remove_symbol),

     // ffi
     make_pair("load-foreign-library", new op_foreign_lib),
     make_pair("get-foreign-symbol", new op_foreign_sym),
     make_pair("internalize", new op_internalize)};

  runtime r;
  r.root = new hydra_module("");
  hydra_symbol* sym = r.root->intern("hydra");
  sym->value = language_module;
  r.active_module = language_module;
  // arithmetic

  for (auto p : inbuilts) {
    hydra_symbol* sym = r.active_module->intern(p.first);
    sym->value = p.second;
  }


  hydra_istream *stm = new hydra_istream();
  stm->stream = &cin;
  sym = r.active_module->intern("cin");
  sym->value = stm;

  string in = "(eval (read (open-file \"../hydra/lang.hd\")))";
  hydra_string *str = new hydra_string();
  str->value = in;
  hydra_object *ast;
  hydra_object *out;

  try {
  ast = read(str, r);
  out = ast->eval(r);
  } catch (string e) {
    cout << e << endl;
  } catch (const char *err) {
    cout << err << endl;
  }

  while (!(in == "(quit)")) {
    try {
      cout << "> ";
      ast = read(stm, r);
      out = ast->eval(r);
      cout << "* " << out << endl;
    } catch (string e) {
      cout << e << endl;
    } catch (const char *err) {
      cout << err << endl;
    }

    getline(cin, in);
    hydra_object::collect_garbage(r);
  }
  return 0;
}
