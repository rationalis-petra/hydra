#ifndef __HYDRA_OP_CONCURRENCY_HPP
#define __HYDRA_OP_CONCURRENCY_HPP

#include "expressions.hpp"

struct op_thread : hydra_oper {
  op_thread();
  hydra_object* call(hydra_object* alist, runtime &r, lexical_scope& s);
};

struct op_thread_join : hydra_oper {
  op_thread_join();
  hydra_object* call(hydra_object* alist, runtime &r, lexical_scope& s);
};

struct op_thread_id : hydra_oper {
  op_thread_id();
  hydra_object* call(hydra_object* alist, runtime &r, lexical_scope& s);
};

struct op_make_mutex : hydra_oper {
  op_make_mutex();
  hydra_object* call(hydra_object* alist, runtime &r, lexical_scope& s);
};

struct op_make_semaphore : hydra_oper {
  op_make_semaphore();
  hydra_object* call(hydra_object* alist, runtime &r, lexical_scope& s);
};
struct op_get_all_threads : hydra_oper {
  op_get_all_threads();
  hydra_object* call(hydra_object* alist, runtime &r, lexical_scope& s);
};

#endif // __HYDRA_OP_CONCURRENCY_HPP
