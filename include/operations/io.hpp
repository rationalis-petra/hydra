#ifndef __HYDRA_OPERATIONS_IO_HPP
#define __HYDRA_OPERATIONS_IO_HPP

#include "expressions.hpp"

struct op_print : hydra_oper {
  op_print();
  hydra_object *call(hydra_object *body, runtime &r, lexical_scope &s);
};
struct op_open_file : hydra_oper {
  op_open_file();
  hydra_object *call(hydra_object *body, runtime &r, lexical_scope &s);
};
struct op_endp : hydra_oper {
  op_endp();
  hydra_object *call(hydra_object *body, runtime &r, lexical_scope &s);
};

struct op_next : hydra_oper {
  op_next();
  hydra_object *call(hydra_object *body, runtime &r, lexical_scope &s);
};
struct op_peek : hydra_oper {
  op_peek();
  hydra_object *call(hydra_object *body, runtime &r, lexical_scope &s);
};
struct op_close : hydra_oper {
  op_close();
  hydra_object *call(hydra_object *body, runtime &r, lexical_scope &s);
};

#endif // __HYDRA_IO_HPP
