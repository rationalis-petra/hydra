#include "types.hpp"
#include "expressions.hpp"
#include "utils.hpp"

using namespace expr;
using namespace interp;

using std::list;
using std::string;

std::list<Object*> cons_to_list(Object* v) {
  list<Object*> out;
  while (!v->null()) {
    Cons* cns = get_inbuilt<Cons*>(v);
    if (!cns) {
      std::string err = "Interperter Error in cons_to_list: provided something that is not a nil or cons";
      throw err;
    }
    out.push_back(cns->car);
    v = cns->cdr;
  }
  return out;
}

Object* list_to_cons(std::list<Object*> lst) {
  if (lst.empty()) {
    return (Object*) expr::nil::get();
  } else {
    Object* car = lst.front();
    lst.pop_front();
    return (Object*) new expr::Cons(car, list_to_cons(lst));
  }
}

// <- : no accessors
// =  : getter, no setter
// .  : both
// {[:x <- 30]*
//  [:y = -10]
//  [:z . 231]}
// (object
//   [:x 3 nil nil t]
//   [:y -10 t nil t]
//   [:z 231 t t t])

    


Symbol* get_keyword(string str) {
  Object* obj = keyword_module->get(str);
  if (obj->null()) {
    Symbol* sym = keyword_module->intern(str);
    sym->value = (Object*) sym;
    GenericFn* gfn = new GenericFn(":" + str);
    gfn->type = type::Fn::with_all({new type::Any}, new type::Any, new type::Any);
    sym->set_invoker(gfn);
    return sym;
  }
  else {
    return dynamic_cast<Symbol*>(obj);
  }
}

std::string hydra_to_string(expr::Object* obj, LocalRuntime &r, LexicalScope &s) {
  expr::Object* str = gn_to_string->call({obj}, r, s);
  return get_inbuilt<HString*>(str)->value;
}
