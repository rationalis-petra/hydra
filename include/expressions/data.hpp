#include <string>
#include <istream>
#include <list>
#include <vector>

#include "object.hpp"

struct hydra_t : public hydra_object {
  std::string to_string() const;  
};

struct hydra_nil : public hydra_object {
  std::string to_string() const;
  bool null() const;
};

struct hydra_cons : public hydra_object {
  std::string to_string() const; hydra_cons();
  hydra_cons(hydra_object* car, hydra_object* cdr);
  hydra_object* eval(runtime& r);

  hydra_object* car;
  hydra_object* cdr;

  unsigned len();
};

struct hydra_num : public hydra_object {
  hydra_num(int num);
  std::string to_string() const;

  int value;
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
  hydra_char();
  hydra_char(int c);
  std::string to_string() const;
  int value;
};
