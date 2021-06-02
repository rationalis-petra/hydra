
# File Structure
The files (both header and source) follow a structure with 4 directories, each of which
corresponds to a namespace
+ Interpreter: Defines data-structures which are useful for core interpreter functionality,
  mostly garbage collection and lexical scoping.
+ Expressions: Defines classes which represent hydra expressions. Because of the design 
  of hydra, all expressions are also representable as objects within the language.
+ Operations: Function definitions for various inbuilt operations, such as addition, 
  object creation, list/vector operations, etc.
+ Types: Define type-objects, which contain type-checking functions subtype-checking
  functions.
  
In general, there should be a 1:1 correspondence between `.cpp` and `.hpp` files, although
this is violated, e.g. by library headers or headers which simply collect other headers
(e.g. `expressions.hpp`).

# Implementation Overview 

The file `expressions/object.hpp` defines the root of the hierarchy, the `Object` class.
This class (and it's subclasses) define the interface to the language: all programs & 
data-structures within the language are an instance of the `Object` class. 

All instances of object have some common functions, such as `mark_node` (for garbage collection)
and `eval` (for evaluation). Eval generally returns the object it is called on, except for case
of `Cons`, which instead performs a function/macro call.

Possibly one of the most important functions is the `hydra_cast` function: The point of this function
is to allow user-defined objects to be cast into objects the C++ code can use/understand. For example,
an object may inherit from, e.g. `Integer`, and so should be treated as an Integer by the C++. The
`hydra_cast` function tries to ensure this is the case (although it is technically broken, and
will likely remain so for quite some time).

Finally, there is the implementation of inbuilt operators such as `cons` or `+`. These are implemented 
in a standardized manner, although the general idea is that they are encapsulated at a standard
`InbuiltOperator` class, which is initialized with a type and function pointer, then given a (string) name
so it can be accessed in the langauge runtime. This works as follows:
+ In the appropriate header file under `Operations`, e.g. `arithmetic.hpp`
  + Declare a pointer to an `Operator` object, e.g. `Operator* plus` within the `op` namespace
  + Also, declare a function `init_<op_type>`, e.g. `init_arithmetic` with input/return type `void`
+ In the corresponding cpp file:
  + Define a local function, e.g. `do_op_plus` this should be the actual implementation of the function you want!
  + Define the `init_arithmetic` function, and make sure it contains a call of the form `op::plus = new InbuiltOperator(...)`,
    where we pass the name, docstring, type, function pointer and a boolean. This ensures the function is initialized and 
    can be used at runtime.
+ In init/operators.cpp, add a call to `init_arithmetic()` if it is not already there
+ in main.cpp, add a name/value pair, e.g. `{"+", op::plus}`, so op::plus becomes accessible at runtime.
