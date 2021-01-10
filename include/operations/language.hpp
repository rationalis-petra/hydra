#ifndef __LANGUAGE_HPP
#define __LANGUAGE_HPP

#include "expressions.hpp"

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
struct op_progn : hydra_oper {
  op_progn();
  hydra_object *call(hydra_object *body, runtime &r);
};


struct op_quote : hydra_oper {
  op_quote();
  hydra_object *call(hydra_object *body, runtime &r);
};

hydra_object *read(hydra_object *stream, runtime &r);
struct op_read : hydra_oper {
  op_read();
  hydra_object *call(hydra_object *body, runtime &r);
};

hydra_object *compile(hydra_object *body, runtime &r);
struct op_compile : hydra_oper {
  op_compile();
  hydra_object *call(hydra_object *body, runtime &r);
};

struct op_eval : hydra_oper {
  op_eval();
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

#endif // __LANGUAGE_HPP
