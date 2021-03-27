
# File Structure
The files (both header and source) follow a structure with 4 directories, each of which
corresponds to a namespace
+ Interpreter: Defines data-structures which are useful for core interpreter functionality,
  such mostly garbage collection and lexical scoping.
+ Expressions: Defines classes which represent 
+ Operations: Function definitions for various inbuilt operations, such as addition, 
  object creation, ...
+ Types: Define type-objects, which contain type-checking functions, 

# Implementation Overview 

The file `expressions/object.hpp` defines the root of the hierarchy, the `Object` class.
This class (and it's subclasses) define the interface to the language: all programs & 
data-structures within the language are an instance of the `Object` class. This class
contains 

