#ifndef __HYDRA_OPERATIONS_SYSTEM_HPP
#define __HYDRA_OPERATIONS_SYSTEM_HPP

#include "boost/filesystem/operations.hpp"
#include "boost/filesystem/path.hpp"
#include <string>

#include "expressions.hpp"


namespace op {
  void initialize_system();

  extern expr::Operator* get_dir;
  extern expr::Operator* set_dir;

  extern expr::Operator* mk_dir;
  extern expr::Operator* mk_file;
  extern expr::Operator* fs_remove;
  extern expr::Operator* fs_remove_all;
}

#endif

