#ifndef __HYDRA_OPERATIONS_MIRROR_HPP
#define __HYDRA_OPERATIONS_MIRROR_HPP

#include "expressions.hpp"

namespace op {

  void initialize_mirror();

  extern expr::GenericFn* get_slots;
  extern expr::GenericFn* set_slot_parent;
  extern expr::GenericFn* set_slot_val;
  extern expr::GenericFn* get_slot_val;
  extern expr::GenericFn* delete_slot;

  extern expr::GenericFn* get_meta;
  extern expr::GenericFn* set_meta_val;
  extern expr::GenericFn* get_meta_val;
  extern expr::GenericFn* delete_meta;

  extern expr::GenericFn* get_mirror;
} // namespace op

#endif // __LOGIC_HPP
