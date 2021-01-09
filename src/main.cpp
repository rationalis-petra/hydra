#include <iostream>
#include <string>
#include <map>
#include <variant>
#include <fstream>

#include "expressions.hpp"
#include "operations.hpp"

using namespace std;

int main(int argc, char **argv) {
  test();
  runtime r;
  // arithmetic
  r.global_store["+"] = new op_plus();
  r.global_store["-"] = new op_minus();
  r.global_store["*"] = new op_multiply();
  r.global_store["/"] = new op_divide();
  r.global_store[">="] = new op_geq();

  // data
  r.global_store["array"] = new op_arr();
  r.global_store["cons"] = new op_cons();
  r.global_store["car"] = new op_car();
  r.global_store["cdr"] = new op_cdr();
  r.global_store["elt"] = new op_elt();

  // io
  r.global_store["print"] = new op_print();
  r.global_store["open-file"] = new op_open_file();
  r.global_store["close-file"] = new op_close();
  // streams
  r.global_store["peek"] = new op_peek();
  r.global_store["next"] = new op_next();
  r.global_store["endp"] = new op_endp();

  r.global_store["eval"] = new op_eval();
  r.global_store["read"] = new op_read();
  r.global_store["set-macro-character"] = new op_set_mac_char();

  // logic
  r.global_store["="] = new op_eq();
  r.global_store["or"] = new op_or();
  r.global_store["and"] = new op_and();
  r.global_store["not"] = new op_not();

  // language
  r.global_store["if"] = new op_if();
  r.global_store["fn"] = new op_fn();
  r.global_store["mac"] = new op_mac();
  r.global_store["def"] = new op_def();
  r.global_store["quote"] = new op_quote();
  r.global_store["progn"] = new op_progn();
  r.global_store["quit"] = new op_quit();

  hydra_istream *stm = new hydra_istream();
  stm->stream = &cin;
  r.global_store["cin"] = stm;

  string in = "(eval (read (open-file \"../hydra/lang.hd\")))";
  hydra_string *str = new hydra_string();
  str->value = in;
  hydra_object *ast = read(str, r);
  hydra_object *out = ast->eval(r);

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
