# File Structure
The Include Directory has four main components
+ expressions (expressions.hpp, expressions/*) This folder contains the base definitions for the
  objects which are used to build the AST. `hydra_object` is a base class, and it is inherited by,
  e.g. `hydra_num` (for numbers), `hydra_symbol`, etc. Subfiles:
  + concurrency: headers for (unimplemented) concurrency primitives
  + data: data objects like number, symbol, ...
  + foreign: for representing foreign types/values
  + language
  + object: the base `hydra_object` class
+ operations (operations.hpp, operations/*) There is a class `inbuilt_oper`, which is inherited
  by inbuilt operations with operations being both functinos (+, -, >) and language syntax 
  (if, fn, ...). Hence, most of the functionality is implemented here.
  + arithmetic: add, multiply, ...
  + concurrency: for generating/running threads
  + data: for making/manipulating lists/arrays
  + foreign: operations for interating with foreign libraries
  + io: operations for reading files, console, etc.
  + language: operations which implement fundamental lanugage syntax - if, progn, fn, mac
  + logic: and, or, not
  + modules: operations for interacting with modules
+ libs: relatively self-explanatory, contains headers for the libraries (ltdf, libffi) that hydra
  depends on
+ types: the types segment is relatively stunted

Source files: The source files are structured similarly, although there are more files in each
of the operations and expressions category. In addition, there is the addition of the memory 
directory, which contains the (very simple) garbage collector.

In addition, there are the main.cpp and lang.cpp files. lang.cpp contains a file with a constant
string that is a hydra program that defines various macros, functions etc. The main function
generates the modules hydra:core, hydra:io, hydra:foreign, hydra:concurrent and user, then 
inserts all operations (+, -, print, ...) into their respective module.

# Implementation Overview 
The class `hydra_object` is the root of the object hierarchy, and contains several functions:
+ **null**: a simple function that returns true iff the object is an instance of =nil=.
+ **to_string**: function which returns a string representation of the object
+ **eval**: very important, describes how to evaluate an expression within a given runtime
  and lexical scope. 
There is also the `marked` boolean, which is used in conjunction with the `node_list` (a list of
all hydra_object instances) for gabage collection

The `runtime` class encapsulates all global state: a reference to a single runtime instance is
passed around the program. It contains:
+ The root module
+ The current (active) module
+ The readtable (see reader macros in wiki)

The `lexical_scope` class contains a mapping from symbols to values.

The small subclasses of `hydra_object` are mostly values (number, string, ...) and these have
been collected into values.cpp. For other subclasses, it is probably best to check the relevant
section of the docs to understand their function before reading the code.
