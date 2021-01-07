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

struct hydra_object;
struct runtime {
  std::map<std::string, hydra_object*> global_store;
  std::map<char, hydra_object*> readtable;
};

struct hydra_object {
  //std::string docstring;

  virtual bool null() const;
  virtual std::string to_string() const = 0;
  virtual hydra_object* eval(runtime& r);
  hydra_object();
  virtual ~hydra_object();


  // mark & sweep garbage collection
  bool marked;
  static std::list<hydra_object*> node_list; 
  static void collect_garbage(runtime& r);
};

struct hydra_t : public hydra_object {
  std::string to_string() const;  
};

struct hydra_nil : public hydra_object {
  std::string to_string() const;
  bool null() const;
};

struct hydra_cons : public hydra_object {
  std::string to_string() const;
  hydra_object* eval(runtime& r);

  hydra_object* car;
  hydra_object* cdr;

  unsigned len();
};

struct hydra_num : public hydra_object {
  std::string to_string() const;

  int val;
};

struct hydra_oper : public hydra_object {
  std::string to_string() const;
  bool eval_args;

  virtual hydra_object* call(hydra_object* arg_list, runtime& r) = 0; 

protected:
  std::list<hydra_object*> get_arg_list(hydra_object* arg_list, runtime& r);
};

struct user_oper : public hydra_oper {
  std::string to_string() const;
  hydra_object* call(hydra_object* arg_list, runtime& r);

  std::string rest;
  // std::list<std::string> optionals
  // std::list<std::string> keyword-args

  std::list<std::string> arg_names;
  hydra_object* expr;
  user_oper(hydra_object* op_def, bool eval_args);
};

struct hydra_istream : public hydra_object {
  std::string to_string() const;
  std::istream *stream;  
  ~hydra_istream();
};

struct hydra_ostream : public hydra_object {
  std::string to_string() const;
  std::ostream stream;

};

struct hydra_array : public hydra_object {
  std::string to_string() const;
  std::vector<hydra_object*> array;
};

struct hydra_symbol : public hydra_object {
  std::string to_string() const;
  std::string symbol;

  hydra_object* eval(runtime& r);
};

struct hydra_string : public hydra_object {
  std::string to_string() const;
  std::string value;  
};

struct hydra_char : public hydra_object {
  std::string to_string() const;
  char value;
};

std::ostream &operator<<(std::ostream &os, const hydra_object *obj);

#endif // __HYDRA_TYPES_HPP
