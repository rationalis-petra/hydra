#ifndef __EXPRESSIONS_HPP
#define __EXPRESSIONS_HPP

#include "expressions.hpp"

#include <list>

int test();

struct op_plus : hydra_oper {
  op_plus();
  hydra_object *call(hydra_object *arg_list, runtime &r);
};
struct op_minus : hydra_oper {
  op_minus();
  hydra_object *call(hydra_object *arg_list, runtime &r);
};
struct op_multiply : hydra_oper {
  op_multiply();
  hydra_object *call(hydra_object *arg_list, runtime &r);
};
struct op_divide : hydra_oper {
  op_divide();
  hydra_object *call(hydra_object *arg_list, runtime &r);
};
struct op_geq : hydra_oper {
  op_geq();
  hydra_object *call(hydra_object *arg_list, runtime &r);
};

// logic
struct op_eq : hydra_oper {
  op_eq();
  hydra_object *call(hydra_object *arg_list, runtime &r);
};
struct op_and : hydra_oper {
  op_and();
  hydra_object *call(hydra_object *arg_list, runtime &r);
};
struct op_or : hydra_oper {
  op_or();
  hydra_object *call(hydra_object *arg_list, runtime &r);
};
struct op_not : hydra_oper {
  op_not();
  hydra_object *call(hydra_object *arg_list, runtime &r);
};

// language
struct op_if : hydra_oper {
  op_if();
  hydra_object *call(hydra_object *arg_list, runtime &r);
};
struct op_def : hydra_oper {
  op_def();
  hydra_object *call(hydra_object *arg_list, runtime &r);
};
struct op_fn : hydra_oper {
  op_fn();
  hydra_object *call(hydra_object *body, runtime &r);
};
struct op_mac : hydra_oper {
  op_mac();
  hydra_object *call(hydra_object *body, runtime &r);
};
struct op_quote : hydra_oper {
  op_quote();
  hydra_object *call(hydra_object *body, runtime &r);
};
struct op_progn : hydra_oper {
  op_progn();
  hydra_object *call(hydra_object *body, runtime &r);
};
struct op_eval : hydra_oper {
  op_eval();
  hydra_object *call(hydra_object *body, runtime &r);
};

hydra_object *read(hydra_object *stream, runtime &r);
struct op_read : hydra_oper {
  op_read();
  hydra_object *call(hydra_object *body, runtime &r);
};

struct op_set_mac_char : hydra_oper {
  op_set_mac_char();
  hydra_object *call(hydra_object *body, runtime &r);
};

struct op_quit : hydra_oper {
  op_quit();
  hydra_object *call(hydra_object *body, runtime &r);
};

// types...

// data: cons basically pointers!
struct op_cons : hydra_oper {
  op_cons();
  hydra_object *call(hydra_object *body, runtime &r);
};
struct op_car : hydra_oper {
  op_car();
  hydra_object *call(hydra_object *body, runtime &r);
};
struct op_cdr : hydra_oper {
  op_cdr();
  hydra_object *call(hydra_object *body, runtime &r);
};
struct op_arr : hydra_oper {
  op_arr();
  hydra_object *call(hydra_object *body, runtime &r);
};
struct op_elt : hydra_oper {
  op_elt();
  hydra_object *call(hydra_object *body, runtime &r);
};

// io
struct op_print : hydra_oper {
  op_print();
  hydra_object *call(hydra_object *body, runtime &r);
};
struct op_open_file : hydra_oper {
  op_open_file();
  hydra_object *call(hydra_object *body, runtime &r);
};
struct op_endp : hydra_oper {
  op_endp();
  hydra_object *call(hydra_object *body, runtime &r);
};

struct op_next : hydra_oper {
  op_next();
  hydra_object *call(hydra_object *body, runtime &r);
};
struct op_peek : hydra_oper {
  op_peek();
  hydra_object *call(hydra_object *body, runtime &r);
};
struct op_close : hydra_oper {
  op_close();
  hydra_object *call(hydra_object *body, runtime &r);
};
#endif // __EXPRESSIONS_HPP
