
#ifndef __HYDRA_INTERPRETER_EXCEPTION_HPP
#define __HYDRA_INTERPRETER_EXCEPTION_HPP

#include <exception>

#include <list>
#include <string>

#include "ltdl.h"
#include "ffi.h"

#include "expressions/object.hpp"
#include "expressions/handler.hpp"

namespace interp {

// A Control exception is intended to pass control flow
// to a higher-level function, usually when some error 
// has occurred but we don't have enough information to generate
// a comprehensive InterpreterError
struct ControlException : public std::exception {};

struct TypeCheckException : public ControlException {};
struct TooFewException : public TypeCheckException {};
struct TooManyException : public TypeCheckException {};


// Here, we defined exceptions that are accessible from 
// within the language

struct InternalException : public expr::Object {
};

struct InterpreterError : public expr::Object {
  InterpreterError(std::string msg);
};

}


#endif
