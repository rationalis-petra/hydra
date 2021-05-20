#include <list>
#include <string>

#include "expressions.hpp"
#include "operations/types.hpp"
#include "types.hpp"
#include "utils.hpp"

using std::list;
using std::string;

using namespace expr;
using namespace interp;

Object *op_typep(list<Object *> arg_list, LocalRuntime &r, LexicalScope &s) {

  Object *obj = arg_list.back();
  type::Type *type_rep = dynamic_cast<type::Type *>(arg_list.front());

  return type_rep->check_type(obj);
}

Object *op_subtype(list<Object *> arg_list, LocalRuntime &r, LexicalScope &s) {

  type::Type *t1 = dynamic_cast<type::Type *>(arg_list.front());
  type::Type *t2 = dynamic_cast<type::Type *>(arg_list.back());

  // subtype is like <
  // (subtype? t1 t2) is (t1 < t2)
  // i.e. return true if t1 is a subtype of t2
  return t2->subtype(t1);
}

Object *op_type_eq(list<Object *> arg_list, LocalRuntime &r, LexicalScope &s) {
  type::Type *t1 = dynamic_cast<type::Type *>(arg_list.front());
  type::Type *t2 = dynamic_cast<type::Type *>(arg_list.back());

  return t1->equal(t2);
}

Object *op_type_is(list<Object *> arg_list, LocalRuntime &r, LexicalScope &s) {
  return new type::Is(arg_list.front());
}

Object *op_type_derives(list<Object *> arg_list, LocalRuntime &r,
                        LexicalScope &s) {
  return new type::Derives(arg_list.front());
}

Object *op_mk_cons_type(list<Object *> arg_list, LocalRuntime &r,
                        LexicalScope &s) {
  if (arg_list.size() == 1) {
    return new type::Cons(dynamic_cast<type::Type *>(arg_list.front()),
                          new type::Any);
  } else {
    return new type::Cons(dynamic_cast<type::Type *>(arg_list.front()),
                          dynamic_cast<type::Type *>(arg_list.back()));
  }
}

Object *op_mk_vector_type(list<Object *> arg_list, LocalRuntime &r,
                          LexicalScope &s) {
  if (arg_list.size() == 1) {
    return new type::Vector(dynamic_cast<type::Type *>(arg_list.front()));
  } else {
    return new type::Vector(dynamic_cast<type::Type *>(arg_list.front()),
                            get_inbuilt<Integer *>(arg_list.back())->value.get_si());
  }
}

Object *op_mk_tuple_type(list<Object *> arg_list, LocalRuntime &r,
                         LexicalScope &s) {
  std::vector<type::Type *> types;
  auto it = arg_list.end();
  do {
    it--;
    types.push_back(dynamic_cast<type::Type *>(*it));
  } while (it != arg_list.begin());
  return new type::Tuple(types);
}

// optional part
// keyword part
// rest part

enum Mode {Args, Optional, Key1, Key2, Rest, Done};

Object *op_mk_fn_type(list<Object *> arg_list, LocalRuntime &r, LexicalScope &s) {
  type::Fn *fn = new type::Fn;
  list<Object*> types = cons_to_list(arg_list.front()) ;
  fn->return_type = dynamic_cast<type::Type*>(arg_list.back());

  Mode mode = Args;

  Symbol *keyword = keyword_module->intern("key");
  Symbol *optional = keyword_module->intern("optional");
  Symbol *rest = keyword_module->intern("rest");
  Symbol *active_key = nullptr;


  for (Object *obj : types) {
    switch (mode) {
    case Args: {
      if (obj == keyword) {
        mode = Key1;
      } else if (obj == optional) {
        mode = Optional;
      } else if (obj == rest) {
        mode = Rest;
      } else if (type::Type* t = dynamic_cast<type::Type *>(obj)) {
        // finally, we may assume this to be an argument!
        fn->arg_list.push_back(t);
      } else {
        string err = "non-type provided to fn type constructor";
        throw err;
      }
      break;
    case Optional: {
      if (obj == keyword) {
        mode = Key1;
      } else if (obj == rest) {
        mode = Rest;
      } else if (type::Type* t = dynamic_cast<type::Type *>(obj)) {
        fn->optional_list.push_back(t);
      } else {
        string err = "non-type provided to fn type constructor";
        throw err;
      }
    } break;
    case Key1: {
      if (obj == rest) {
        mode = Rest;
      } else if (Symbol* s = dynamic_cast<Symbol *>(obj)) {
        active_key = get_keyword(s->name);
        mode = Key2;
      } else {
        string err = "non-symbol provided to key in fn type constructor";
        throw err;
      }
    } break;
    case Key2: {
      if (type::Type* t = dynamic_cast<type::Type *>(obj)) {
        fn->keywords[active_key] = t;

        active_key = nullptr;
        mode = Key1;
      } else {
        string err = "non-symbol provided to key in fn type constructor";
        throw err;
      }
    }
    break;
    case Rest: {
      if (type::Type* t = dynamic_cast<type::Type *>(obj)) {
        fn->rest_type = t;
      } else {
        string err = "non-type provided to fn type constructor";
        throw err;
      }
      mode = Done;
    } break;
    case Done: {
      string err = "More than one argument provided to :rest in type constructor";
      throw err;
    } break;
    }
    }
  }

  for (Object *obj : arg_list) {
    Object::collector->remove_root(obj);
  }
  return fn;
}


Object *op_mk_typeclass(list<Object *> arg_list, LocalRuntime &r, LexicalScope &s) { 
  type::TypeClass* cls = new type::TypeClass;

  for (Object* arg : arg_list) {
    list<Object*> lst = cons_to_list(arg);
    type::Fn* fnc = get_inbuilt<type::Fn*>(op::mk_fn_type->call(lst, r, s));
    cls->ops.push_back(fnc);
  }

  return cls;
}

Operator *op::typep;
Operator *op::subtype;
Operator *op::type_eq;
Operator *op::mk_is;
Operator *op::mk_derives;
Operator *op::mk_cons_type;
Operator *op::mk_tuple_type;
Operator *op::mk_vector_type;
Operator *op::mk_fn_type;
Operator *op::mk_typeclass;

// You'll notice that all types are nullptr this is because
// some global type variables  require the type ops to be
// initialized, so we initialize them first.
//
// Then, once we have initialized the types, we return
// and add type annotations to our type operations.

void op::initialize_type_ops() {

  op::mk_typeclass = new InbuiltOperator(
      "class",
      "Creates a new typeclass\n",
      op_mk_typeclass, nullptr, true);

  op::typep = new InbuiltOperator(
                                  "type?",
      "Returns t if the first argument is the type defined by the\n"
      "second, otherwise returns nil",
      op_typep, nullptr, true);

  // op::mk_type = new InbuiltOperator(
  //     "Calls the constructor for a particular type", op_mk_type,
  //     type::Fn::with_all({new type::MetaConstructor}, new type::Any,
  //                        new type::MetaType),
  //     true);

  op::subtype = new InbuiltOperator(
                                    "subtype?",
      "Returns true if the first argument is a subtype of the second",
      op_subtype, nullptr, true);

  op::type_eq =
    new InbuiltOperator("type =", "Equality for types", op_type_eq, nullptr, true);

  op::mk_is =
    new InbuiltOperator("Is", "Constructs an Is type", op_type_is, nullptr, true);

  op::mk_derives = new InbuiltOperator("Derives", "Constructs a Derives type",
                                       op_type_derives, nullptr, true);

  op::mk_tuple_type = new InbuiltOperator("Tuple", "Constructs a Tuple Type",
                                          op_mk_tuple_type, nullptr, true);

  op::mk_vector_type = new InbuiltOperator("Vector", "Constructs a vector type",
                                           op_mk_vector_type, nullptr, true);

  op::mk_cons_type = new InbuiltOperator("Cons", "Constructs a cons type",
                                         op_mk_cons_type, nullptr, true);

  op::mk_fn_type =
    new InbuiltOperator("Fn", "Constructs a fn type", op_mk_fn_type, nullptr, true);
}

//
void op::type_type_ops() {
  op::mk_typeclass->type =
      type::Fn::with_rest(new type::List);

  op::typep->type = type::Fn::with_args({new type::MetaType, new type::Any});

  op::subtype->type =
      type::Fn::with_args({new type::MetaType, new type::MetaType});

  op::type_eq->type =
      type::Fn::with_args({new type::MetaType, new type::MetaType});

  op::mk_is->type = type::Fn::with_args({new type::Any});

  op::mk_derives->type = type::Fn::with_args({new type::Any});

  op::mk_tuple_type->type = type::Fn::with_rest(new type::MetaType);

  op::mk_vector_type->type =
      type::Fn::with_args({new type::MetaType, type::integer_type});
  op::mk_cons_type->type =
    type::Fn::with_args_optional({}, {new type::MetaType, new type::MetaType});

  op::mk_fn_type->type =
      type::Fn::with_args({new type::List, new type::MetaType});
}
