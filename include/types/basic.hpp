#ifndef __HYDRA_TYPE_BASIC_HPP
#define __HYDRA_TYPE_BASIC_HPP

#include <list>

#include "types/type.hpp"

struct type_derives : public hydra_type {
  void mark_node();
  hydra_object_object* object;
  std::string to_string() const;
  virtual hydra_object *check_type(hydra_object *obj);
  hydra_type* constructor(std::list<hydra_object*> lst);
};

struct type_nil : public hydra_type {
  virtual void mark_node();
  std::string to_string() const;
  virtual hydra_object *check_type(hydra_object *obj);
  hydra_type* constructor(std::list<hydra_object*> lst);
};

struct type_integer : public hydra_type {
  virtual void mark_node();
  std::string to_string() const;
  hydra_object* check_type(hydra_object* obj);
  hydra_type* constructor(std::list<hydra_object*> lst);
};

struct type_string : public hydra_type {
  virtual void mark_node();
  std::string to_string() const;
  hydra_object* check_type(hydra_object* obj);
  hydra_type* constructor(std::list<hydra_object*> lst);
};

struct type_module : public hydra_type {
  virtual void mark_node();
  std::string to_string() const;
  hydra_object* check_type(hydra_object* obj);
  hydra_type* constructor(std::list<hydra_object*> lst);
};

struct type_char : public hydra_type {
  virtual void mark_node();
  std::string to_string() const;
  hydra_object* check_type(hydra_object* obj);
  hydra_type* constructor(std::list<hydra_object*> lst);
};

struct type_iostream : public hydra_type {
  virtual void mark_node();
  std::string to_string() const;
  hydra_object* check_type(hydra_object* obj);
  hydra_type* constructor(std::list<hydra_object*> lst);
};

struct type_istream : public hydra_type {
  virtual void mark_node();
  std::string to_string() const;
  hydra_object* check_type(hydra_object* obj);
  hydra_type* constructor(std::list<hydra_object*> lst);
};

struct type_ostream : public hydra_type {
  virtual void mark_node();
  std::string to_string() const;
  hydra_object* check_type(hydra_object* obj);
  hydra_type* constructor(std::list<hydra_object*> lst);
};

struct type_type : public hydra_type {
  virtual void mark_node();
  std::string to_string() const;
  hydra_object* check_type(hydra_object* obj);
  hydra_type* constructor(std::list<hydra_object*> lst);
};

#endif
