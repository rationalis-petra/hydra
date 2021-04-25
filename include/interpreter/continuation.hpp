#ifndef __HYDRA_INTERPRETER_CONTINUATION_HPP
#define __HYDRA_INTERPRETER_CONTINUATION_HPP

#include "runtime.hpp"

#include <list>
#include <string>

#include "ltdl.h"
#include "ffi.h"
namespace expr{
  struct Object;
}

namespace interp {

  struct Continuation {
    expr::Object* obj;
    LexicalScope s;

    expr::Object* complete(LocalRuntime& r);
  };

}


#endif
