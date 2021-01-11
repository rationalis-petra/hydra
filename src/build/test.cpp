#include <iostream>

#include <string>
#include <string.h>

#include "ltdl.h"
#include "ffi.h"

using namespace std;

lt_dlhandle glfw_lib;

void test_glfw_init() {
  cerr << "init" << endl;
  ffi_cif init_interface;
  ffi_type *arg_types[0];
  void *arg_values[0];


  int open_succ = lt_dlinit();
  if (!(open_succ == 0)) {
    string err = "Error initializing ltdl";
    throw err;
  }

  glfw_lib = lt_dlopen("libglfw.so");
  if (glfw_lib == NULL) {
    cerr << "cannot open library" << endl;
  }

  if (ffi_prep_cif(&init_interface, FFI_DEFAULT_ABI, 0, &ffi_type_sint, arg_types) != FFI_OK) {
    cout << "ffi not preped for test_glfw_init" << endl;
  }

  void* address = lt_dlsym(glfw_lib, "glfwInit");
  if (address == nullptr) {
    cout << "Cannot get address of glfwInit";
  }


  ffi_arg result;
  ffi_call(&init_interface, FFI_FN(address), &result, arg_values);
  cout << "glfw_int: " << (int) result << endl;
}

void test_glfw_window() {
  ffi_cif window_interface;
  ffi_type *arg_types[5];
  void *arg_values[5];

  arg_types[0] = &ffi_type_sint;;
  arg_types[1] = &ffi_type_sint;;
  arg_types[2] = &ffi_type_pointer;;
  arg_types[3] = &ffi_type_pointer;;
  arg_types[4] = &ffi_type_pointer;;

  int width = 800;
  int height = 600;
  const char* str = "hello";
  void* mynull = nullptr;
  arg_values[0] = &width;
  arg_values[1] = &height;
  arg_values[2] = &str;
  arg_values[3] = &mynull;
  arg_values[4] = &mynull;

  if (ffi_prep_cif(&window_interface, FFI_DEFAULT_ABI, 5, &ffi_type_sint, arg_types) != FFI_OK) {
    cout << "ffi not preped for test_glfw_window" << endl;
  }


  void* address = lt_dlsym(glfw_lib, "glfwCreateWindow");
  if (address == nullptr) {
    cout << "Cannot get address of glfwCreateWindow";
  }

  ffi_arg result;
  ffi_call(&window_interface, FFI_FN(address), &result, arg_values);
  cout << "glfw_int: " << (void*) result << endl;
}
