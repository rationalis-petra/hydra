#ifndef __HYDRA_TYPE_TYPE_HPP
#define __HYDRA_TYPE_TYPE_HPP

#include <list>

#include "expressions/object.hpp"


struct hydra_type : public hydra_object {
public:
  virtual hydra_object* check_type(hydra_object* obj) = 0;
};


#endif
