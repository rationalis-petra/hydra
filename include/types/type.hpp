#ifndef __HYDRA_TYPE_TYPE_HPP
#define __HYDRA_TYPE_TYPE_HPP

#include <list>

#include "expressions/object.hpp"


struct hydra_type : public hydra_object {
public:
  virtual hydra_object* check_type(hydra_object* obj) = 0;
};

struct type_nil : public hydra_type {
  virtual void mark_node();
  std::string to_string() const;
  virtual hydra_object* check_type(hydra_object* obj);};

struct type_integer : public hydra_type {
  virtual void mark_node();
  std::string to_string() const;
  hydra_object* check_type(hydra_object* obj);
};

struct type_string : public hydra_type {
  virtual void mark_node();
  std::string to_string() const;
  hydra_object* check_type(hydra_object* obj);
};

struct type_symbol : public hydra_type {
  virtual void mark_node();
  std::string to_string() const;
  hydra_object* check_type(hydra_object* obj);
};

struct type_cons : public hydra_type {
  virtual void mark_node();
  std::string to_string() const;
  hydra_object* check_type(hydra_object* obj);
};

struct type_vector : public hydra_type {
  virtual void mark_node();
  std::string to_string() const;
  hydra_object* check_type(hydra_object* obj);
};

struct type_list : public hydra_type {
  virtual void mark_node();
  std::string to_string() const;
  hydra_object* check_type(hydra_object* obj);
};

struct type_module : public hydra_type {
  virtual void mark_node();
  std::string to_string() const;
  hydra_object* check_type(hydra_object* obj);
};

struct type_char : public hydra_type {
  virtual void mark_node();
  std::string to_string() const;
  hydra_object* check_type(hydra_object* obj);
};

struct type_iostream : public hydra_type {
  virtual void mark_node();
  std::string to_string() const;
  hydra_object* check_type(hydra_object* obj);
};

struct type_istream : public hydra_type {
  virtual void mark_node();
  std::string to_string() const;
  hydra_object* check_type(hydra_object* obj);
};

struct type_ostream : public hydra_type {
  virtual void mark_node();
  std::string to_string() const;
  hydra_object* check_type(hydra_object* obj);
};

struct type_type : public hydra_type {
  virtual void mark_node();
  std::string to_string() const;
  hydra_object* check_type(hydra_object* obj);
};

#endif
