#ifndef __HYDRA_EXPRESSIONS_MESSAGE_HPP
#define __HYDRA_EXPRESSIONS_MESSAGE_HPP

#include <map>
#include <set>
#include <string>
#include <functional>

#include "object.hpp"

// Here, we re-envision functions as messages 
// There is no lexical context?? only objects....
namespace expr {

//

struct Method : public Object {
  //std::unordered_map<Symbol*, Object*> context;
  Object* expr;
};

struct Role : public Object {
  Symbol* s;
};

struct NumRole : public Role {
  unsigned arg_num;
};
struct KeyRole : public Role {};

struct Message : public Object {
  Symbol* s;
  Role* r;
};

} // namespace expr

#endif
