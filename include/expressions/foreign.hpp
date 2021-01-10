#ifndef __HYDRA_FOREIGN_HPP
#define __HYDRA_FOREIGN_HPP

#include <list>
#include <string>

#include "ltdl.h"
#include "ffi.h"

#include "object.hpp"
#include "types.hpp"

struct hydra_foreign_lib : public hydra_object {
  hydra_foreign_lib(lt_dlhandle lib);
  std::string to_string() const;

  lt_dlhandle lib;
};

struct hydra_foreign_sym : public hydra_object {
  hydra_foreign_sym(void *addr);
  std::string to_string() const;

  void* address;
};

struct hydra_foreign_oper : public hydra_oper {
  hydra_object* call(hydra_object* args, runtime& r);
  hydra_foreign_oper();

  std::list<foreign_type> arg_types;
  foreign_type return_type;
  ffi_cif fn_def;
  void(*fn_address)(void);
};

#endif // __HYDRA_FOREIGN_HPP
