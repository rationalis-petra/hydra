#include <fstream>
#include <iostream>
#include <list>
#include <string>

#include "expressions.hpp"
#include "operations.hpp"
#include "types.hpp"

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

Value *op_print(Operator *op, Value *alist, LocalRuntime &r, LexicalScope &s) {
  list<Value *> arg_list = op->get_arg_list(alist, r, s);

  if (HString *str = dynamic_cast<HString *>(arg_list.front())) {
    cout << str->value;
  } else {
    cout << arg_list.front()->to_string();
  }

  Value::roots.remove(arg_list.front());
  return arg_list.front();
}

Operator *op::print = new InbuiltOperator(
    "Prints a string representation of the provided argument to\n"
    "the standard output stream",
    op_print, type::Fn::with_args({new type::Any}), true);

Value *op_open_file(Operator *op, Value *alist, LocalRuntime &r,
                    LexicalScope &s) {
  list<Value *> arg_list = op->get_arg_list(alist, r, s);
  Value *filename = arg_list.front();
  Value* out;

  Symbol *method;
  if (arg_list.size() == 2) {
    method = dynamic_cast<Symbol *>(arg_list.back());
  } else {
    method = new Symbol("both");
  }
  if (HString *strfilename = dynamic_cast<HString *>(filename)) {
    if (method->name == "input") {
      Istream *outs = new Istream();
      outs->stream = new ifstream(strfilename->value);
      out = outs;
    } else if (method->name == "output") {
      Ostream *outs = new Ostream();
      outs->stream = new ofstream(strfilename->value);
      out = outs;
    } else if (method->name == "both") {
      IOstream *outs = new IOstream();
      outs->stream = new fstream(strfilename->value);
      out = outs;
    } else {
      string err = "Invalid argument to open: " + method->name;
      throw err;
    }
  } else {
    string err = "Inavalid argument to open-file: " + filename->to_string();
    throw err;
  }
  for (Value* v : arg_list) {
    Value::roots.remove(v);
  }
  return out;
}

Operator *op::open_file = new InbuiltOperator(
    "Takes a string (path), and returns an input stream located\n"
    "by the path",
    op_open_file,
    type::Fn::with_args_optional({new type::TString}, {new type::Symbol}),
    true);

Value *op_next(Operator *op, Value *alist, LocalRuntime &r, LexicalScope &s) {
  list<Value *> arg_list = op->get_arg_list(alist, r, s);

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

  for (Value* v : arg_list) {
    Value::roots.remove(v);
  }  

  return new Char(ch);
}

Operator *op::next = new InbuiltOperator(
    "Takes an input stream, advances it one character\n"
    "and returns the character at the 'current' position",
    op_next,
    type::Fn::with_all({new type::Istream}, nullptr, new type::Istream),
    true);

Value *op_peek(Operator *op, Value *alist, LocalRuntime &r, LexicalScope &s) {
  list<Value *> arg_list = op->get_arg_list(alist, r, s);

  istream *stream;
  if (Istream *s = dynamic_cast<Istream *>(arg_list.front())) {
    stream = s->stream;
  } else if (IOstream *s = dynamic_cast<IOstream *>(arg_list.front())) {
    stream = s->stream;
  } else {
    string err = "Non-istream argument provided to peek";
    throw err;
  }
  for (Value* v : arg_list) {
    Value::roots.remove(v);
  }

  char ch = stream->peek();
  return new Char(ch);
}

Operator *op::peek = new InbuiltOperator(
    "Takes an input stream, and peeks at the next character\n"
    "but does not advance the input stream",
    op_peek, type::Fn::with_all({new type::Istream}, nullptr, new type::Char),
    true);

Value *op_put(Operator *op, Value *alist, LocalRuntime &r, LexicalScope &s) {
  list<Value *> arg_list = op->get_arg_list(alist, r, s);

  Ostream *stream = dynamic_cast<Ostream *>(arg_list.front());
  Char *c = dynamic_cast<Char *>(arg_list.back());

  stream->stream->put(c->value);

  return stream;
}

Operator *op::put = new InbuiltOperator(
    "Takes an output stream, a character, and puts the\n"
    "character in the output stream's current position",
    op_put, type::Fn::with_all({new type::Ostream}, nullptr, new type::Char),
    true);

Value *op_endp(Operator *op, Value *alist, LocalRuntime &r, LexicalScope &s) {
  list<Value *> arg_list = op->get_arg_list(alist, r, s);
  if (arg_list.size() != 1) {
    string err = "Invalid number of arguments provided to endp";
    throw err;
  }

  if (Istream *stream = dynamic_cast<Istream *>(arg_list.front())) {
    istream *file = static_cast<istream *>(stream->stream);
    if (file->eof()) {
      return t::get();
    } else {
      return nil::get();
    }
  } else {
    string err = "Non-istream argument provided to end";
    throw err;
  }
}

Operator *op::endp = new InbuiltOperator(
    "Returns t if a given input stream has reached the\n"
    "end of the file, and nil otherwise",
    op_endp, type::Fn::with_args({new type::Istream}), true);

Value *op_close(Operator *op, Value *alist, LocalRuntime &r, LexicalScope &s) {
  list<Value *> arg_list = op->get_arg_list(alist, r, s);
  if (arg_list.size() != 1) {
    string err = "Invalid number of arguments provided to close";
    throw err;
  }

  if (Istream *stream = dynamic_cast<Istream *>(arg_list.front())) {
    ifstream *file = static_cast<ifstream *>(stream->stream);
    file->close();
    return nil::get();
  } else {
    string err = "Non-istream argument provided to peek";
    throw err;
  }
}

Operator *op::close = new InbuiltOperator(
    "Takes an input stream representing a file, and closes it", op_close,
    type::Fn::with_args({new type::IOStream}), true);
