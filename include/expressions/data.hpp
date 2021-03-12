#include <string>
#include <istream>
#include <list>
#include <vector>

#include "object.hpp"

namespace expr {
struct t : public Value {
  virtual void mark_node();

  std::string to_string() const;  
  static t* get();
private:
  static t* singleton;
  t();
};

struct nil : public Value {
  virtual void mark_node();

  std::string to_string() const;
  static nil* get();
  bool null() const;
private:
  static nil* singleton;
  nil();
};

struct Cons : public Value {
  virtual void mark_node();

  std::string to_string() const; 
  Cons(Value* car, Value* cdr);
  Value* eval(LocalRuntime& r, LexicalScope& s);

  Value* car;
  Value* cdr;

  unsigned len();
};

struct Integer : public Value {
  virtual void mark_node();

  Integer(int num);
  std::string to_string() const;

  int value;
};

struct Istream : public Value {
  virtual void mark_node();

  std::string to_string() const;
  std::istream *stream;  
  ~Istream();
};

struct Ostream : public Value {
  virtual void mark_node();

  std::string to_string() const;
  std::ostream *stream;
  ~Ostream();
};

struct IOstream : public Value {
  virtual void mark_node();

  std::string to_string() const;
  std::iostream *stream;
  ~IOstream();
};

struct Vector : public Value {
  virtual void mark_node();

  std::string to_string() const;
  std::vector<Value*> array;
};


struct HString : public Value {
  virtual void mark_node();

  HString();
  HString(std::string str);

  std::string to_string() const;
  std::string value;  
};

struct Char : public Value {
  virtual void mark_node();

  Char();
  Char(int c);
  std::string to_string() const;
  int value;
};

struct Tuple : public Value {
  virtual void mark_node();

  std::string to_string() const;
  std::vector<Value*> values;
};

struct Symbol;

struct Union : public Value {
  virtual void mark_node();

  std::string to_string() const;
  Symbol* tag;
  Value* value;
};

}
