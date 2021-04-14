#include <string>
#include <typeinfo>

#include "expressions.hpp"

using namespace expr;
using namespace interp;

Reader::Reader(Object *obj, Symbol *_slot) : slot(_slot) {
  type->return_type = new type::Any;
  type->rest_type = nullptr;
  type->arg_list = {new type::Is(obj)};
}

Object *Reader::call(std::list<Object *> arg_list, interp::LocalRuntime &r,
                     interp::LexicalScope &s, bool) {
  Object* obj = arg_list.front();
  return obj->slots[slot];
}


Writer::Writer(Object *obj, Symbol *_slot) : slot(_slot){
  type->return_type = new type::Any;
  type->rest_type = nullptr;
  type->arg_list = {new type::Is(obj), new type::Any};
}

Object *Writer::call(std::list<Object *> arg_list, interp::LocalRuntime &r,
                     interp::LexicalScope &s, bool) {
  Object* obj = arg_list.front();
  obj->slots[slot] = arg_list.back();
  return arg_list.back();
}
