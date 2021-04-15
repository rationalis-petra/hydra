#include <math.h>
#include <string>
#include <typeinfo>

#include "expressions.hpp"

using namespace std;
using namespace expr;
using namespace interp;

Parent* Number::parent;

Parent* Integer::parent;

string Integer::to_string(LocalRuntime &r, LexicalScope &s) {
  return std::to_string(value);
}

Integer::Integer(int num) : value(num) {
  Symbol* pt = keyword_module->intern("parent");
  parents.insert(pt);
  slots[pt] = parent;
  value = num;
  numtype = hinteger;
}

Number* Integer::add(Number* other) {
  switch(other->numtype) {
  case hinteger: {
    Integer* i = static_cast<Integer*>(other);
    return new Integer(value + i->value);
    }
    break;
  case hfloat: {
    Float* i = static_cast<Float*>(other);
    return new Float(value + i->value);
    }
    break;
  }
  string err = "integer-add: case exhaustion";
  throw err;
}
Number* Integer::minus(Number* other) {
  switch(other->numtype) {
  case hinteger: {
    Integer* i = static_cast<Integer*>(other);
    return new Integer(value - i->value);
    }
    break;
  case hfloat: {
    Float* i = static_cast<Float*>(other);
    return new Float(value - i->value);
    }
    break;
  }
  string err = "integer-minus: case exhaustion";
  throw err;
}
Number* Integer::multiply(Number* other) {
  switch(other->numtype) {
  case hinteger: {
    Integer* i = static_cast<Integer*>(other);
    return new Integer(value * i->value);
    }
    break;
  case hfloat: {
    Float* i = static_cast<Float*>(other);
    return new Float(value * i->value);
    }
    break;
  }
  string err = "integer-mult: case exhaustion";
  throw err;
}
Number* Integer::divide(Number* other) {
  switch(other->numtype) {
  case hinteger: {
    Integer* i = static_cast<Integer*>(other);
    return new Integer(value / i->value);
    }
    break;
  case hfloat: {
    Float* i = static_cast<Float*>(other);
    return new Float(value / i->value);
    }
    break;
  }
  string err = "integer-div: case exhaustion";
  throw err;
}
Number* Integer::sqrt() {
  return new Float(::sqrt(value));
}


Parent* Float::parent;

string Float::to_string(LocalRuntime &r, LexicalScope &s) {
  return std::to_string(value);
}

Float::Float(double num) : value(num) {
  Symbol* pt = keyword_module->intern("parent");
  parents.insert(pt);
  slots[pt] = parent;
  value = num;
  numtype = hfloat;
}

Number* Float::add(Number* other) {
  switch(other->numtype) {
  case hinteger: {
    Integer* i = static_cast<Integer*>(other);
    return new Float(value + i->value);
    }
    break;
  case hfloat: {
    Float* i = static_cast<Float*>(other);
    return new Float(value + i->value);
    }
    break;
  }
  string err = "float-add: case exhaustion";
  throw err;
}
Number* Float::minus(Number* other) {
  switch(other->numtype) {
  case hinteger: {
    Integer* i = static_cast<Integer*>(other);
    return new Float(value - i->value);
    }
    break;
  case hfloat: {
    Float* i = static_cast<Float*>(other);
    return new Float(value - i->value);
    }
    break;
  }
  string err = "float-minus: case exhaustion";
  throw err;
}
Number* Float::multiply(Number* other) {
  switch(other->numtype) {
  case hinteger: {
    Integer* i = static_cast<Integer*>(other);
    return new Float(value * i->value);
    }
    break;
  case hfloat: {
    Float* i = static_cast<Float*>(other);
    return new Float(value * i->value);
    }
    break;
  }
  string err = "float-mult: case exhaustion";
  throw err;
}
Number* Float::divide(Number* other) {
  switch(other->numtype) {
  case hinteger: {
    Integer* i = static_cast<Integer*>(other);
    return new Float(value / i->value);
    }
    break;
  case hfloat: {
    Float* i = static_cast<Float*>(other);
    return new Float(value / i->value);
    }
    break;
  }
  string err = "float-div: case exhaustion";
  throw err;
}

Number* Float::sqrt() {
  return new Float(::sqrt(value));
}
