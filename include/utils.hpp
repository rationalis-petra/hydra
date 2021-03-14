#include <list>

#include "expressions.hpp"

std::list<expr::Object*> cons_to_list(expr::Object* v);
expr::Object* list_to_cons(std::list<expr::Object*> v);

expr::Symbol* get_keyword(std::string str);
