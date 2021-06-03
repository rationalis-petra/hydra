#include <math.h>
#include <string>
#include <typeinfo>

#include "expressions.hpp"
#include "operations/data.hpp"
#include "utils.hpp"

using namespace std;
using namespace expr;
using namespace interp;

Parent *Number::parent;
Parent *Real::parent;

Parent *Integer::parent;

string Integer::to_string(LocalRuntime &r, LexicalScope &s) {
  return value.get_str();
}

Integer::Integer() {
  Symbol *pt = keyword_module->intern("parent");
  parents.insert(pt);
  slots[pt] = parent;

  mpz_init(value);

  numtype = hinteger;
}
Integer::Integer(long num) {
  Symbol *pt = keyword_module->intern("parent");
  parents.insert(pt);
  slots[pt] = parent;

  mpz_init(value);
  mpz_set_si(value, num);

  numtype = hinteger;
}

Number *Integer::add(Number *other) {
  switch (other->numtype) {
  case hinteger: {
    Integer *i = static_cast<Integer *>(other);
    Integer* out = new Integer();
    mpz_add(out->value, value, i->value);
    return out;
  } break;
  case hfloat: {
    Float *i = static_cast<Float *>(other);
    return new Float(mpz_get_d(value) + i->value);
  } break;
  case hcomplex: {
    Complex *cmplx = static_cast<Complex *>(other);
    return new Complex(add(cmplx->num1), cmplx->num2);
  } break;
  }
  string err = "integer-add: case exhaustion";
  throw err;
}

Number *Integer::minus(Number *other) {
  switch (other->numtype) {
  case hinteger: {
    Integer *i = static_cast<Integer *>(other);
    Integer *out = new Integer;
    mpz_sub(out->value, value, i->value);
    return out;
  } break;
  case hfloat: {
    Float *i = static_cast<Float *>(other);
    return new Float(mpz_get_d(value) - i->value);
  } break;
  case hcomplex: {
    Complex *cmplx = static_cast<Complex *>(other);
    return new Complex(minus(cmplx->num1), cmplx->num2);
  } break;
  }

  string err = "integer-minus: case exhaustion";
  throw err;
}
Number *Integer::multiply(Number *other) {
  switch (other->numtype) {
  case hinteger: {
    Integer *i = static_cast<Integer *>(other);
    Integer *out = new Integer;
    mpz_mul(out->value, value, i->value);
    return out;
  } break;
  case hfloat: {
    Float *i = static_cast<Float *>(other);
    return new Float(mpz_get_d(value) * i->value);
  } break;
  case hcomplex: {
    Complex *cmplx = static_cast<Complex *>(other);
    return new Complex(multiply(cmplx->num1), multiply(cmplx->num2));
  } break;
  }
  string err = "integer-mult: case exhaustion";
  throw err;
}
Number *Integer::divide(Number *other) {
  switch (other->numtype) {
  case hinteger: {
    Integer *i = static_cast<Integer *>(other);
    Integer *out = new Integer;
    mpz_cdiv_q(out->value, value, i->value);
    return out;
  } break;
  case hfloat: {
    Float *i = static_cast<Float *>(other);
    return new Float(mpz_get_d(value) / i->value);
  } break;
  case hcomplex: {
    Complex *cmplx = static_cast<Complex *>(other);
    Number *denominator = cmplx->num1->multiply(cmplx->num1)
                              ->add(cmplx->num2->multiply(cmplx->num2));
    return new Complex(
        multiply(cmplx->num1)->divide(denominator),
        multiply(cmplx->num2)->divide(denominator)->multiply(new Integer(-1)));
  } break;
  }
  string err = "integer-div: case exhaustion";
  throw err;
}
Number *Integer::sqrt() { return new Float(::sqrt(mpz_get_si(value))); }

signed long Integer::get_sl() {
  return mpz_get_si(value);
}

unsigned long Integer::get_ul() {
  return mpz_get_ui(value);
}
























Parent *Float::parent;

string Float::to_string(LocalRuntime &r, LexicalScope &s) {
  return std::to_string(value);
}

Float::Float(double num) : value(num) {
  Symbol *pt = keyword_module->intern("parent");
  parents.insert(pt);
  slots[pt] = parent;
  value = num;
  numtype = hfloat;
}

Number *Float::add(Number *other) {
  switch (other->numtype) {
  case hinteger: {
    Integer *i = static_cast<Integer *>(other);
    return new Float(value + mpz_get_d(i->value));
  } break;
  case hfloat: {
    Float *i = static_cast<Float *>(other);
    return new Float(value + i->value);
  } break;
  case hcomplex: {
    Complex *cmplx = static_cast<Complex *>(other);
    return new Complex(add(cmplx->num1), cmplx->num2);
  } break;
  }
  string err = "float-add: case exhaustion";
  throw err;
}
Number *Float::minus(Number *other) {
  switch (other->numtype) {
  case hinteger: {
    Integer *i = static_cast<Integer *>(other);
    return new Float(value - mpz_get_d(i->value));
  } break;
  case hfloat: {
    Float *i = static_cast<Float *>(other);
    return new Float(value - i->value);
  } break;
  case hcomplex: {
    Complex *cmplx = static_cast<Complex *>(other);
    return new Complex(minus(cmplx->num1), cmplx->num2);
  } break;
  }
  string err = "float-minus: case exhaustion";
  throw err;
}
Number *Float::multiply(Number *other) {
  switch (other->numtype) {
  case hinteger: {
    Integer *i = static_cast<Integer *>(other);
    return new Float(value * mpz_get_d(i->value));
  } break;
  case hfloat: {
    Float *i = static_cast<Float *>(other);
    return new Float(value * i->value);
  } break;
  case hcomplex: {
    Complex *cmplx = static_cast<Complex *>(other);
    return new Complex(multiply(cmplx->num1), multiply(cmplx->num2));
  } break;
  }
  string err = "float-mult: case exhaustion";
  throw err;
}
Number *Float::divide(Number *other) {
  switch (other->numtype) {
  case hinteger: {
    Integer *i = static_cast<Integer *>(other);
    return new Float(value / mpz_get_d(i->value));
  } break;
  case hfloat: {
    Float *i = static_cast<Float *>(other);
    return new Float(value / i->value);
  } break;
  case hcomplex: {
    Complex *cmplx = static_cast<Complex *>(other);
    Number *denominator = cmplx->num1->multiply(cmplx->num1)
                              ->add(cmplx->num2->multiply(cmplx->num2));
    return new Complex(
        multiply(cmplx->num1)->divide(denominator),
        multiply(cmplx->num2)->divide(denominator)->multiply(new Integer(-1)));
  } break;
  }
  string err = "float-div: case exhaustion";
  throw err;
}

Number *Float::sqrt() { return new Float(::sqrt(value)); }



















Parent* Complex::parent;

Complex::Complex(Number *_num1, Number *_num2)  {
  num1 = dynamic_cast<Real*>(_num1);
  num2 = dynamic_cast<Real*>(_num2);
  if (!(num1 && num2)) {
    string err = "Error constructing complex: non-real argument provided";
    throw err;
  }

  Symbol *pt = keyword_module->intern("parent");
  parents.insert(pt);
  slots[pt] = parent;
  numtype = hcomplex;
}

Number *Complex::add(Number *other) {
  switch (other->numtype) {
  case hinteger: {
    Integer *i = static_cast<Integer *>(other);
    return new Complex(num1->add(i), num2);
  } break;
  case hfloat: {
    Float *i = static_cast<Float *>(other);
    return new Complex(num1->add(i), num2);
  } break;
  case hcomplex: {
    Complex *cmplx = static_cast<Complex *>(other);
    return new Complex(num1->add(cmplx->num1), num2->add(cmplx->num2));
  } break;
  }
  string err = "complex-add: case exhaustion";
  throw err;
}
Number *Complex::minus(Number *other) {
  switch (other->numtype) {
  case hinteger: {
    Integer *i = static_cast<Integer *>(other);
    return new Complex(num1->minus(i), num2);
  } break;
  case hfloat: {
    Float *i = static_cast<Float *>(other);
    return new Complex(num1->minus(i), num2);
  } break;
  case hcomplex: {
    Complex *cmplx = static_cast<Complex *>(other);
    return new Complex(num1->minus(cmplx->num1), num2->minus(cmplx->num2));
  } break;
  }
  string err = "complex-minus: case exhaustion";
  throw err;
}
Number *Complex::multiply(Number *other) {
  switch (other->numtype) {
  case hinteger: {
    Integer *i = static_cast<Integer *>(other);
    return new Complex(num1->multiply(i), num2->multiply(i));
  } break;
  case hfloat: {
    Float *i = static_cast<Float *>(other);
    return new Complex(num1->multiply(i), num2->multiply(i));
  } break;
  case hcomplex: {
    Complex *cmplx = static_cast<Complex *>(other);
    return new Complex(
        num1->multiply(cmplx->num1)->minus(num2->multiply(cmplx->num2)),
        num1->multiply(cmplx->num2)->add(num2->multiply(cmplx->num1)));
  } break;
  }
  string err = "complex-mult: case exhaustion";
  throw err;
}
Number *Complex::divide(Number *other) {
  switch (other->numtype) {
  case hinteger: {
    Integer *i = static_cast<Integer *>(other);
    return new Complex(num1->divide(i), num2->divide(i));
  } break;
  case hfloat: {
    Float *i = static_cast<Float *>(other);
    return new Complex(num1->divide(i), num2->divide(i));
  } break;
  case hcomplex: {
    Complex *cmplx = static_cast<Complex *>(other);
    Number *denominator = cmplx->num1->multiply(cmplx->num1)
                              ->add(cmplx->num2->multiply(cmplx->num2));
    return new Complex(
        num1->multiply(cmplx->num1)
            ->add(num2->multiply(cmplx->num2)->divide(denominator)),
        num2->multiply(cmplx->num1)
            ->minus(num1->multiply(cmplx->num2))
            ->divide(denominator));
  } break;
  }
  string err = "complex-mult: case exhaustion";
  throw err;
}

string Complex::to_string(LocalRuntime &r, LexicalScope &s) {
  return "(" + hydra_to_string(num1, r, s) + " + " +
         hydra_to_string(num2, r, s) + "i)";
}

void Complex::mark_node() {
  if (marked) return;
  Object::mark_node();
  num1->mark_node();
  num2->mark_node();
}
