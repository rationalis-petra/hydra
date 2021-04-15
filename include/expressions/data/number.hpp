#ifndef __HYDRA_EXPRESSIONS_DATA_NUMBER_HPP
#define __HYDRA_EXPRESSIONS_DATA_NUMBER_HPP

#include <string>
#include <istream>
#include <list>
#include <vector>

#include "expressions/object.hpp"
#include "expressions/parent.hpp"

namespace expr {

enum NumberType {hinteger, hfloat};

struct Number : public Object {
  static Parent* parent;
  NumberType numtype;
  virtual Number* sqrt() = 0;
  virtual Number* add(Number* other) = 0;
  virtual Number* minus(Number* other) = 0;
  virtual Number* multiply(Number* other) = 0;
  virtual Number* divide(Number* other) = 0;
};

struct Integer : public Number {
  static Parent* parent;

  Integer(int num);
  std::string to_string(interp::LocalRuntime &r, interp::LexicalScope& s);

  int value;
  virtual Number* sqrt();
  virtual Number* add(Number* other);
  virtual Number* minus(Number* other);
  virtual Number* multiply(Number* other);
  virtual Number* divide(Number* other);
};

struct Float : public Number {
  static Parent* parent;

  Float(double num);
  std::string to_string(interp::LocalRuntime &r, interp::LexicalScope& s);

  double value;
  virtual Number* sqrt();
  virtual Number* add(Number* other);
  virtual Number* minus(Number* other);
  virtual Number* multiply(Number* other);
  virtual Number* divide(Number* other);
};


}

#endif
