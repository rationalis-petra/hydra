#include <string>
#include <map>

#include "object.hpp"

struct hydra_symbol;
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

  hydra_symbol* rest; // for accepting variable arguments
  hydra_symbol* self; // name of function within function's scope
  // std::list<std::string> optionals
  // std::list<std::string> keyword-args

  std::list<hydra_symbol*> arg_names;
  hydra_object* expr;
  user_oper(hydra_object* op_def, bool eval_args);
};


// modules are like packages or namepsaces in other languages
struct hydra_module : public hydra_object {
  hydra_module();
  hydra_module(std::string name);
  std::string to_string() const;
  std::map<std::string, hydra_symbol*> symbols;
  std::map<std::string, hydra_symbol*> exported_symbols;

  std::string name;

  hydra_object* get(std::string str);
  hydra_object* get(std::list<std::string> str);
  hydra_symbol* intern(std::string str);
  hydra_symbol* intern(std::list<std::string> str);
};

struct hydra_symbol : public hydra_object {
  hydra_object* value;
  std::string name;

  //std::list<hydra_module*> modules;

  hydra_symbol(std::string name);
  std::string to_string() const;
  hydra_object* eval(runtime& r);
};
