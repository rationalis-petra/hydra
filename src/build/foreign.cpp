#include <string>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "ffi.h"

using std::string;

int test() {
  ffi_cif cif;
  ffi_type *args[1];
  void *values[1];
  char* s = (char*) malloc(20);
  ffi_arg rc;

  /* Initialize the argument info vectors */
  args[0] = &ffi_type_pointer;
  values[0] = &s;

  /* Initialize the cif */
  if (ffi_prep_cif(&cif, FFI_DEFAULT_ABI, 1, &ffi_type_sint, args) == FFI_OK) {
    strcpy(s, string("Hello World!").c_str());
    ffi_call(&cif, (void(*)()) &puts, &rc, values);
    /* rc now holds the result of the call to puts */

    /* values holds a pointer to the function's arg, so to
       call puts() again all we need to do is change the
       value of s */
    strcpy(s, string("This is cool!").c_str());
    ffi_call(&cif, (void(*)()) &puts, &rc, values);
  }

  return 0;
}
