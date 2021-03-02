#ifndef __HYDRA_OPERATIONS_CONDITIONS_HPP
#define __HYDRA_OPERATIONS_CONDITIONS_HPP

#include "expressions.hpp"


struct op_handler_bind : public hydra_oper {
  op_handler_bind();
  hydra_object* call(hydra_object* alist, runtime &r, lexical_scope& s);
};

struct op_handler_catch : public hydra_oper {
  op_handler_catch();
  hydra_object* call(hydra_object* alist, runtime &r, lexical_scope& s);
};

struct op_signal_condition : public hydra_oper {
  op_signal_condition();
  hydra_object* call(hydra_object* alist, runtime &r, lexical_scope& s);
};

// jump to the restart...

struct op_add_restart : public hydra_oper {
  op_add_restart();
  hydra_object* call(hydra_object* alist, runtime &r, lexical_scope& s);
};

struct op_get_restarts : public hydra_oper {
  op_get_restarts();
  hydra_object* call(hydra_object* alist, runtime &r, lexical_scope& s);
};


#endif
