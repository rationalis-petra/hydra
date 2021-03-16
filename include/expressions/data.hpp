#include <string>
#include <istream>
#include <list>
#include <vector>

#include "object.hpp"

namespace expr {
struct t : public Object {
  virtual void mark_node();

  std::string to_string() const;  
  static t* get();
private:
  static t* singleton;
  t();
};

struct nil : public Object {
  virtual void mark_node();

  std::string to_string() const;
  static nil* get();
  bool null() const;
private:
  static nil* singleton;
  nil();
};

struct Cons : public Object {
  virtual void mark_node();

  std::string to_string() const; 
  Cons(Object* car, Object* cdr);
  Object* eval(LocalRuntime& r, LexicalScope& s);

  Object* car;
  Object* cdr;

  unsigned len();
};

struct IntegerClass : public Object {
  std::string to_string() const;
};
struct Integer : public Object {
  static IntegerClass* parent;

  Integer(int num);
  std::string to_string() const;

  int value;
};

struct Istream : public Object {
  virtual void mark_node();

  std::string to_string() const;
  std::istream *stream;  
  ~Istream();
};

struct Ostream : public Object {
  virtual void mark_node();

  std::string to_string() const;
  std::ostream *stream;
  ~Ostream();
};

struct IOstream : public Object {
  virtual void mark_node();

  std::string to_string() const;
  std::iostream *stream;
  ~IOstream();
};

struct Vector : public Object {
  // expose vector access
  Object* operator[](int i);
  int size();


  virtual void mark_node();

  std::string to_string() const;
  std::vector<Object*> array;
};


struct HString : public Object {
  virtual void mark_node();

  HString();
  HString(std::string str);

  std::string to_string() const;
  std::string value;  
};

struct Char : public Object {
  virtual void mark_node();

  Char();
  Char(int c);
  std::string to_string() const;
  int value;
};

struct Tuple : public Object {
  virtual void mark_node();

  // expose vector access
  Object* operator[](int i);
  int size();

  std::string to_string() const;
  std::vector<Object*> values;
};

struct Symbol;

struct Union : public Object {
  virtual void mark_node();

  std::string to_string() const;
  Symbol* tag;
  Object* value;
};

}
