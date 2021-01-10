#include <string>
#include <map>

#include "object.hpp"

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

  std::string rest; // for accepting variable arguments
  std::string self; // name of function within function's scope
  // std::list<std::string> optionals
  // std::list<std::string> keyword-args

  std::list<std::string> arg_names;
  hydra_object* expr;
  user_oper(hydra_object* op_def, bool eval_args);
};


// modules are like packages or namepsaces in other languages
struct hydra_module : public hydra_object {
  std::string to_string() const;

  std::map<std::string, hydra_object*> store;
};
