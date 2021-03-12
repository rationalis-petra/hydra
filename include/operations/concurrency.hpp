#ifndef __HYDRA_OPERATIONS_CONCURRENCY_HPP
#define __HYDRA_OPERATIONS_CONCURRENCY_HPP

#include "expressions.hpp"

namespace op {

  extern expr::Operator* thread;
  extern expr::Operator* thread_join;
  extern expr::Operator* thread_id;
  extern expr::Operator* make_mutex;
  extern expr::Operator* make_semaphore;
  extern expr::Operator* get_all_threads;

} // namespace op
#endif // __HYDRA_OP_CONCURRENCY_HPP
