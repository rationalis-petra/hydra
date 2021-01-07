#ifndef __IVY_TYPES_HPP
#define __IVY_TYPES_HPP
// TYPES defines the ivy_object, which is the internal representation of a
// compiled ivy object: the primary purpose of it is 

#include <list>
#include <string>
#include <vector>
#include <iostream>
#include <map>

// the portable ivy standard libraries are written in terms
// of a select group of types and operations

struct ivy_object;
struct runtime {
  std::map<std::string, ivy_object*> global_store;
  std::map<char, ivy_object*> readtable;
};






struct ivy_object {
  //std::string docstring;

  virtual bool null() const;
  virtual std::string to_string() const = 0;
  virtual ivy_object* eval(runtime& r);
  ivy_object();
  virtual ~ivy_object();


  // mark & sweep garbage collection
  bool marked;
  static std::list<ivy_object*> node_list; 
  static void collect_garbage(runtime& r);
};

struct ivy_t : public ivy_object {
  std::string to_string() const;  
};

struct ivy_nil : public ivy_object {
  std::string to_string() const;
  bool null() const;
};

struct ivy_cons : public ivy_object {
  std::string to_string() const;
  ivy_object* eval(runtime& r);

  ivy_object* car;
  ivy_object* cdr;

  unsigned len();
};

struct ivy_num : public ivy_object {
  std::string to_string() const;

  int val;
};

struct ivy_oper : public ivy_object {
  std::string to_string() const;
  bool eval_args;

  virtual ivy_object* call(ivy_object* arg_list, runtime& r) = 0; 

protected:
  std::list<ivy_object*> get_arg_list(ivy_object* arg_list, runtime& r);
};

struct user_oper : public ivy_oper {
  std::string to_string() const;
  ivy_object* call(ivy_object* arg_list, runtime& r);

  std::list<std::string> arg_names;
  ivy_object* expr;
  user_oper(ivy_object* op_def, bool eval_args);
};

struct ivy_istream : public ivy_object {
  std::string to_string() const;
  std::istream *stream;  
  ~ivy_istream();
};

struct ivy_ostream : public ivy_object {
  std::string to_string() const;
  std::ostream stream;

};

struct ivy_array : public ivy_object {
  std::string to_string() const;
  std::vector<ivy_object*> array;
};

struct ivy_symbol : public ivy_object {
  std::string to_string() const;
  std::string symbol;

  ivy_object* eval(runtime& r);
};

struct ivy_string : public ivy_object {
  std::string to_string() const;
  std::string value;  
};

struct ivy_char : public ivy_object {
  std::string to_string() const;
  char value;
};

std::ostream &operator<<(std::ostream &os, const ivy_object *obj);

#endif // __IVY_TYPES_HPP
