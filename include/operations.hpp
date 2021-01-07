#ifndef __EXPRESSIONS_HPP
#define __EXPRESSIONS_HPP

#include "expressions.hpp"

#include <list>

struct op_plus : ivy_oper {
  op_plus();
  ivy_object *call (ivy_object* arg_list, runtime& r);
};
struct op_minus : ivy_oper {
  op_minus();
  ivy_object *call (ivy_object* arg_list, runtime& r);
};
struct op_multiply : ivy_oper {
  op_multiply();
  ivy_object *call (ivy_object* arg_list, runtime& r);
};
struct op_divide : ivy_oper {
  op_divide();
  ivy_object *call (ivy_object* arg_list, runtime& r);
};
struct op_geq : ivy_oper {
  op_geq();
  ivy_object *call (ivy_object* arg_list, runtime& r);
};

// logic
struct op_eq : ivy_oper {
  op_eq();
  ivy_object *call (ivy_object* arg_list, runtime& r);
};
struct op_and : ivy_oper {
  op_and();
  ivy_object *call (ivy_object* arg_list, runtime& r);
};
struct op_or : ivy_oper {
  op_or();
  ivy_object *call (ivy_object* arg_list, runtime& r);
};
struct op_not : ivy_oper {
  op_not();
  ivy_object *call (ivy_object* arg_list, runtime& r);
};

// language
struct op_if : ivy_oper {
  op_if();
  ivy_object *call (ivy_object* arg_list, runtime& r);
};
struct op_def : ivy_oper {
  op_def();
  ivy_object *call (ivy_object* arg_list, runtime& r);
};
struct op_fn : ivy_oper {
  op_fn();
  ivy_object *call (ivy_object* body, runtime& r);
};
struct op_mac : ivy_oper {
  op_mac();
  ivy_object *call (ivy_object* body, runtime& r);
};
struct op_quote : ivy_oper {
  op_quote();
  ivy_object *call (ivy_object* body, runtime& r);
};
struct op_progn : ivy_oper {
  op_progn();
  ivy_object *call (ivy_object* body, runtime& r);
};
struct op_eval : ivy_oper {
  op_eval();
  ivy_object *call (ivy_object* body, runtime& r);
};

ivy_object* read(ivy_object* stream, runtime& r);
struct op_read : ivy_oper {
  op_read();
  ivy_object *call (ivy_object* body, runtime &r);
};


// types...

// data: cons basically pointers!
struct op_cons : ivy_oper {
  op_cons();
  ivy_object *call (ivy_object* body, runtime& r);
};
struct op_car : ivy_oper { 
  op_car();
  ivy_object *call (ivy_object* body, runtime& r);
};
struct op_cdr : ivy_oper {
  op_cdr();
  ivy_object *call (ivy_object* body, runtime& r);
};
struct op_arr : ivy_oper {
  op_arr();
  ivy_object *call(ivy_object *body, runtime &r);
};

// io
struct op_print : ivy_oper {
  op_print();
  ivy_object * call(ivy_object *body, runtime &r);
};
struct op_open_file : ivy_oper {
  op_open_file();
  ivy_object *call (ivy_object* body, runtime &r);
};

struct op_next : ivy_oper {
  op_next();
  ivy_object * call(ivy_object *body, runtime &r);
};
struct op_peek : ivy_oper {
  op_peek();
  ivy_object * call(ivy_object *body, runtime &r);
};
struct op_close : ivy_oper {
  op_close();
  ivy_object * call(ivy_object *body, runtime &r);
};
#endif// __EXPRESSIONS_HPP
