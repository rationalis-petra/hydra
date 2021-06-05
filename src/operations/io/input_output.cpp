#include <fstream>
#include <iostream>
#include <list>
#include <string>

#include "expressions.hpp"
#include "operations.hpp"
#include "types.hpp"
#include "utils.hpp"

using std::cout;
using std::list;
using std::string;
// streams
using std::fstream;
using std::ifstream;
using std::iostream;
using std::istream;
using std::ofstream;
using std::ostream;

using namespace expr;
using namespace interp;

Object *op_print(list<Object *> arg_list, LocalRuntime &r, LexicalScope &s) {
  for (Object *obj : arg_list) {
    if (HString *str = dynamic_cast<HString *>(obj)) {
      cout << str->value;
    } else {
      cout << hydra_to_string(obj, r, s);
    }
    Object::collector->remove_root(obj);
  }
  return arg_list.back();
}

Object *op_open_file(list<Object *> arg_list, LocalRuntime &r,
                     LexicalScope &s) {

  Object *filename = arg_list.front();
  Object *out;

  Symbol *method;
  if (arg_list.size() == 2) {
    method = dynamic_cast<Symbol *>(arg_list.back());
  } else {
    method = new Symbol("both");
  }
  if (HString *strfilename = dynamic_cast<HString *>(filename)) {
    if (method->name == "input") {
      Istream *outs = new Istream();
      ifstream *file = new ifstream(strfilename->value);
      if (file->is_open()) {
        outs->stream = file;
        out = outs;
      } else {
        out = nil::get();
      }
    } else if (method->name == "output") {
      Ostream *outs = new Ostream();
      ofstream *file = new ofstream(strfilename->value);
      if (file->is_open()) {
        outs->stream = file;
        out = outs;
      } else {
        out = nil::get();
      }
    } else if (method->name == "both") {
      IOstream *outs = new IOstream();
      fstream *file = new fstream(strfilename->value);
      if (file->is_open()) {
        outs->stream = file;
        out = outs;
      } else {
        out = nil::get();
      }
    } else {
      string err = "Invalid argument to open: " + method->name;
      throw err;
    }
  } else {
    string err =
        "Inavalid argument to open-file: " + hydra_to_string(filename, r, s);
    throw err;
  }
  for (Object *v : arg_list) {
    Object::collector->remove_root(v);
  }
  return out;
}

Object *op_next(list<Object *> arg_list, LocalRuntime &r, LexicalScope &s) {

  istream *stream;
  if (Istream *s = dynamic_cast<Istream *>(arg_list.front())) {
    stream = s->stream;
  } else if (IOstream *s = dynamic_cast<IOstream *>(arg_list.front())) {
    stream = s->stream;
  } else {
    string err = "Non-istream argument provided to next";
    throw err;
  }

  char ch;
  stream->get(ch);

  for (Object *v : arg_list) {
    Object::collector->remove_root(v);
  }

  return new Char(ch);
}

Object *op_peek(list<Object *> arg_list, LocalRuntime &r, LexicalScope &s) {

  istream *stream;
  if (Istream *s = dynamic_cast<Istream *>(arg_list.front())) {
    stream = s->stream;
  } else if (IOstream *s = dynamic_cast<IOstream *>(arg_list.front())) {
    stream = s->stream;
  } else {
    string err = "Non-istream argument provided to peek";
    throw err;
  }
  for (Object *v : arg_list) {
    Object::collector->remove_root(v);
  }

  char ch = stream->peek();
  return new Char(ch);
}

Object *op_put(list<Object *> arg_list, LocalRuntime &r, LexicalScope &s) {

  Ostream *stream = dynamic_cast<Ostream *>(arg_list.front());
  Char *c = dynamic_cast<Char *>(arg_list.back());

  stream->stream->put(c->value);

  return stream;
}

Object *op_endp(list<Object *> arg_list, LocalRuntime &r, LexicalScope &s) {

  istream *strm;
  if (Istream *stream = dynamic_cast<Istream *>(arg_list.front())) {
    strm = static_cast<istream *>(stream->stream);
  } else if (IOstream *stream = dynamic_cast<IOstream *>(arg_list.front())) {
    strm = static_cast<istream *>(stream->stream);
  } else {
    string err = "Non-istream argument provided to end";
    throw err;
  }

  if (strm->eof()) {
    return t::get();
  } else {
    return nil::get();
  }
}

Object *op_close(list<Object *> arg_list, LocalRuntime &r, LexicalScope &s) {

  if (Istream *stream = dynamic_cast<Istream *>(arg_list.front())) {
    ifstream *file = static_cast<ifstream *>(stream->stream);
    file->close();
    return nil::get();
  } else if (Ostream *stream = dynamic_cast<Ostream *>(arg_list.front())) {
    ofstream *file = static_cast<ofstream *>(stream->stream);
    file->close();
    return nil::get();
  } else if (IOstream *stream = dynamic_cast<IOstream *>(arg_list.front())) {
    fstream *file = static_cast<fstream *>(stream->stream);
    file->close();
    return nil::get();
  } else {
    string err = "Non-stream argument provided to close";
    throw err;
  }
}

Object *op_write_ostream(list<Object *> arg_list, LocalRuntime &r, LexicalScope &s) {
  std::ostream* stream = nullptr;
  if (Ostream* ost = get_inbuilt<Ostream*>(arg_list.front())) {
    stream = ost->stream;
  } else if (IOstream* iost = get_inbuilt<IOstream*>(arg_list.front())) {
    stream = iost->stream;
  } 
  arg_list.pop_front();
  for (Object* obj : arg_list) {
    *stream << hydra_to_string(obj, r, s);
  }
  return arg_list.back();
}

GenericFn *op::peek;
GenericFn *op::next;
GenericFn *op::put;
GenericFn *op::write;
GenericFn *op::endp;

Operator *op::open_file;
Operator *op::print;

GenericFn *op::close;

// TODO: stream type??

void op::initialize_io() {
  Operator* in_file_closei = new InbuiltOperator(
      "close", "Takes an input stream representing a file, and closes it",
      op_close, type::Fn::with_args({type::istream_type}), true);
  Operator* in_file_closeo = new InbuiltOperator(
      "close", "Takes an input stream representing a file, and closes it",
      op_close, type::Fn::with_args({type::ostream_type}), true);
  op::close = new GenericFn("close");
  op::close->type = type::Fn::with_args({new type::Any});
  op::close->add(in_file_closei);
  op::close->add(in_file_closeo);

  op::print = new InbuiltOperator(
      "print",
      "Prints a string representation of the provided argument to\n"
      "the standard output stream",
      op_print, type::Fn::with_rest(new type::Any), true);

  op::open_file = new InbuiltOperator(
      "open-file",
      "Takes a string (path), and returns an input stream located\n"
      "by the path",
      op_open_file,
      type::Fn::with_args_optional({type::string_type}, {type::symbol_type}),
      true);

  Operator* in_op_next = new InbuiltOperator(
      "next",
      "Takes an input stream, advances it one character\n"
      "and returns the character at the 'current' position",
      op_next,
      type::Fn::with_all({type::istream_type}, nullptr, type::istream_type),
      true);
  op::next = new GenericFn("next");
  op::next->type =
      type::Fn::with_all({type::istream_type}, nullptr, type::istream_type);
  op::next->add(in_op_next);

  Operator* in_op_peek = new InbuiltOperator(
      "peek",
      "Takes an input stream, and peeks at the next character\n"
      "but does not advance the input stream",
      op_peek,
      type::Fn::with_all({type::istream_type}, nullptr, type::character_type),
      true);
  op::peek = new GenericFn("peek");
  op::peek->type =
    type::Fn::with_all({new type::Any}, nullptr, new type::Any);
  op::peek->add(in_op_peek);

  Operator *in_op_put = new InbuiltOperator(
      "put",
      "Takes an output stream, a character, and puts the\n"
      "character in the output stream's current position",
      op_put,
      type::Fn::with_all({type::ostream_type}, type::character_type, type::character_type),
      true);
  op::put = new GenericFn("put");
  op::put->type =
    type::Fn::with_all({new type::Any}, new type::Any, new type::Any);
  op::put->add(in_op_put);

  Operator* op_in_end = new InbuiltOperator(
      "end?",
      "Returns t if a given input stream has reached the\n"
      "end of the file, and nil otherwise",
      op_endp, type::Fn::with_args({type::istream_type}), true);
  op::endp = new GenericFn("end?");
  op::endp->type = type::Fn::with_args({new type::Any});
  op::endp->add(op_in_end);



  Operator *in_op_write = new InbuiltOperator(
      "write",
      "Takes an output stream, an object(s), and puts the\n"
      "object(s) in the output stream's current position",
      op_write_ostream,
      type::Fn::with_all({type::ostream_type}, new type::Any, new type::Any),
      true);

  op::write = new GenericFn("write");
  op::write->type =
    type::Fn::with_all({new type::Any}, new type::Any, new type::Any);
  op::write->add(in_op_write);

}
