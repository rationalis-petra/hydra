#include "operations.hpp"
#include "types.hpp"
#include "init.hpp"

void init_operators() {
  // SETUP Operators
  // we have operators on types, which is required to create some types,
  // but operators need to be annotated with types. Hence, the first three
  // functions here are special: initialize_type_ops initializes the operators
  // but does not add type annotations. Once that is done, we can create (initialze)
  // the types, then add annotations to the operatos which operate on types...
  op::initialize_type_ops();
  type::initialize_types();
  op::type_type_ops();

  // this are more simple - simply assigning the operatos like '+' and
  // 'to-string' to global variables 
  // these two define generic functions that are inserted into by subsequent
  // initializations
  op::initialize_logic();
  op::initialize_data();

  // these define "singleton" functions (generic functions with only one value)
  op::initialize_arithmetic();
  op::initialize_mirror();
  op::initialize_user_obj();
  op::initialize_cons();
  op::initialize_string();
  op::initialize_vector();
  op::initialize_tuple();
  op::initialize_dev();
  op::initialize_conditions();
  op::initialize_language();
  op::initialize_module();
  op::initialize_mkoperator();
  op::initialize_foreign();
  op::initialize_foreign_proxy();
  op::initialize_concurrency();
  op::initialize_system();

  // io contains generics needed by other io  
  // operators, such as netowrking
  op::initialize_io();
  op::initialize_network();

  // we place read last because it uses other operations
  op::initialize_read();

}
