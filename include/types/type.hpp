#ifndef __HYDRA_TYPE_TYPE_HPP
#define __HYDRA_TYPE_TYPE_HPP

#include <list>
#include <vector>
#include <string>

#include "expressions/object.hpp"
#include "expressions/oobject.hpp"
#include "expressions/operation.hpp"


struct hydra_type : public hydra_object {
public:
  virtual hydra_object* check_type(hydra_object* obj) = 0;
  virtual hydra_type* constructor(std::list<hydra_object*> lst) = 0;
};

struct type_symbol : public hydra_type {
  virtual void mark_node();
  std::string to_string() const;
  hydra_object* check_type(hydra_object* obj);
  hydra_type* constructor(std::list<hydra_object*> lst);
};

// TODO: TUPLE, UNION!!!

struct type_cons : public hydra_type {
  virtual void mark_node();
  std::string to_string() const;
  hydra_object* check_type(hydra_object* obj);
  hydra_type* constructor(std::list<hydra_object*> lst);

  hydra_type* type_car;
  hydra_type* type_cdr;
};

struct type_vector : public hydra_type {
  virtual void mark_node();
  std::string to_string() const;
  hydra_object* check_type(hydra_object* obj);
  hydra_type* constructor(std::list<hydra_object*> lst);

  hydra_type* type_elt;
};

struct type_list : public hydra_type {
  virtual void mark_node();
  std::string to_string() const;
  hydra_object* check_type(hydra_object* obj);
  hydra_type* constructor(std::list<hydra_object*> lst);

  hydra_type* elt_type;
};

struct type_tuple : public hydra_type {
  virtual void mark_node();
  std::string to_string() const;
  hydra_object* check_type(hydra_object* obj);
  hydra_type* constructor(std::list<hydra_object*> lst);

  std::vector<hydra_type*> types;
};

struct type_union : public hydra_type {
  virtual void mark_node();
  std::string to_string() const;
  hydra_object* check_type(hydra_object* obj);
  hydra_type* constructor(std::list<hydra_object*> lst);

  std::list<hydra_type*> types;
};

struct type_derives : public hydra_type {
  type_derives();
  void mark_node();
  hydra_object_object* object;
  std::string to_string() const;
  virtual hydra_object *check_type(hydra_object *obj);
  hydra_type* constructor(std::list<hydra_object*> lst);
};

struct type_eql : public hydra_type {
  type_eql();
  void mark_node();
  hydra_object* object;
  std::string to_string() const;
  virtual hydra_object *check_type(hydra_object *obj);
  hydra_type* constructor(std::list<hydra_object*> lst);
};
#endif
