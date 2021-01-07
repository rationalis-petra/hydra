#include <iostream>
#include <string>
#include <unordered_map>
#include <variant>
#include <fstream>

#include "expressions.hpp"
#include "operations.hpp"

using namespace std;

int main(int argc, char **argv) {
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

  // io
  r.global_store["print"] = new op_print();
  r.global_store["open"] = new op_open_file();
  r.global_store["peek"] = new op_peek();
  r.global_store["next"] = new op_next();
  r.global_store["close"] = new op_close();

  r.global_store["eval"] = new op_eval();
  r.global_store["read"] = new op_read();

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

  // eval (def defn (mac (name arg-list body) (def name (fn arg-list body))))
  string in = "(eval (read (open \"../hydra/lang.hd\")))";

  while (!(in == "(quit)")) {
    // cerr << ast << endl;
    try {
      ivy_string* str = new ivy_string();
      str->value = in;
      ivy_object* ast = read(str, r);
      ivy_object* out = ast->eval(r);
      cout << "* " << out << endl;
    } catch (string e) {
      cout << e << endl;
    } catch (const char *err) {
      cout << err << endl;
    }

    cout << "> ";
    getline(cin, in);
    ivy_object::collect_garbage(r);
  }
  return 0;
}
