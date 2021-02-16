#include <iostream>
#include <fstream>
#include <string>
#include <list>

#include "types.hpp"
#include "expressions.hpp"
#include "operations.hpp"

using std::list;
using std::string;
using std::cout;
using std::endl;
// streams
using std::ifstream;
using std::istream;
using std::iostream;
using std::fstream;
using std::ofstream;
using std::ostream;

op_print::op_print() {
  is_fn = true;
  docstring = new hydra_string("Prints a string representation of the provided argument to\n"
                               "the standard output stream");
  type->arg_list.push_front(new type_nil);
}
hydra_object *op_print::call(hydra_object *alist, runtime &r, lexical_scope &s) {
  list<hydra_object *> arg_list = get_arg_list(alist, r, s);
  if (arg_list.size() != 1) {
    throw "Invalid number of arguments to print: print takes 1 argument";
  }
  if (hydra_string* str = dynamic_cast<hydra_string*>(arg_list.front())) {
    cout << str->value;
  }
  else {
    cout << arg_list.front()->to_string();
  }
  return arg_list.front();
}

op_open_file::op_open_file() {
  is_fn = true;
  docstring = new hydra_string("Takes a string (path), and returns an input stream located\n"
                               "by the path");
  type->arg_list.push_front(new type_string);
  type->optional_list.push_front(new type_symbol);
}
hydra_object *op_open_file::call(hydra_object *alist, runtime &r, lexical_scope& s) {
  list<hydra_object *> arg_list = get_arg_list(alist, r, s);
  hydra_object *filename = arg_list.front();

  hydra_symbol* method;
  if (arg_list.size() == 2) {
    method = dynamic_cast<hydra_symbol*>(arg_list.back());
  } else {
    method = new hydra_symbol("both");
  }
  if (hydra_string *strfilename = dynamic_cast<hydra_string *>(filename)) {
    if (method->name == "input") {
      hydra_istream *out = new hydra_istream();
      out->stream = new ifstream(strfilename->value);
      return out;
    } else if (method->name == "output") {
      hydra_ostream *out = new hydra_ostream();
      out->stream = new ofstream(strfilename->value);
      return out;
    } else if (method->name == "both") {
      hydra_iostream *out = new hydra_iostream();
      out->stream = new fstream(strfilename->value);
      return out;
    } else {
      string err = "Invalid argument to open: " + method->name;
      throw err;
    }
  } else {
    string err = "Inavalid argument to open-file: " + filename->to_string();
    throw err;
  }
}

op_next::op_next() {
  is_fn = true;
  docstring = new hydra_string("Takes an input stream, advances it one character\n"
                               "and returns the character at the 'current' position");
  type->arg_list.push_front(new type_istream);
}

hydra_object *op_next::call(hydra_object* alist, runtime& r, lexical_scope& s) {
  list<hydra_object*> arg_list = get_arg_list(alist, r, s);

  istream* stream;
  if (hydra_istream* s = dynamic_cast<hydra_istream*>(arg_list.front())) {
    stream = s->stream;
  } else if (hydra_iostream* s = dynamic_cast<hydra_iostream*>(arg_list.front())) {
    stream = s->stream;
  } else {
    string err = "Non-istream argument provided to next";
    throw err;
  }

  char ch;
  stream->get(ch);
  return new hydra_char(ch);
}

op_peek::op_peek() {
  is_fn = true;
  docstring = new hydra_string("Takes an input stream, and peeks at the next character\n"
                               "but does not advance the input stream");
  type->arg_list.push_front(new type_istream);
}

hydra_object *op_peek::call(hydra_object* alist, runtime& r, lexical_scope &s) {
  list<hydra_object*> arg_list = get_arg_list(alist, r, s);

  istream* stream;
  if (hydra_istream* s = dynamic_cast<hydra_istream*>(arg_list.front())) {
    stream = s->stream;
  } else if (hydra_iostream* s = dynamic_cast<hydra_iostream*>(arg_list.front())) {
    stream = s->stream;
  } else {
    string err = "Non-istream argument provided to peek";
    throw err;
  }

  char ch = stream->peek();
  return new hydra_char(ch);
}

op_put::op_put() {
  is_fn = true;
  docstring = new hydra_string("Takes an output stream, a character, and puts the\n"
                               "character in the output stream's current position");
  type->arg_list.push_front(new type_char);
  type->arg_list.push_front(new type_ostream);
}

hydra_object *op_put::call(hydra_object* alist, runtime& r, lexical_scope &s) {
  list<hydra_object*> arg_list = get_arg_list(alist, r, s);

  hydra_ostream *stream = dynamic_cast<hydra_ostream*>(arg_list.front());
  hydra_char *c = dynamic_cast<hydra_char*>(arg_list.back());

  stream->stream->put(c->value);

  return stream;
}

op_endp::op_endp() {
  is_fn = true;
  docstring = new hydra_string("Returns t if a given input stream has reached the\n"
                               "end of the file, and nil otherwise");
  type->arg_list.push_front(new type_istream);
}

hydra_object *op_endp::call(hydra_object* alist, runtime& r, lexical_scope &s) {
  list<hydra_object*> arg_list = get_arg_list(alist, r, s);
  if (arg_list.size() != 1) {
    string err = "Invalid number of arguments provided to endp"; 
    throw err;
  }

  if (hydra_istream* stream = dynamic_cast<hydra_istream*>(arg_list.front())) {
    istream* file = static_cast<istream*>(stream->stream);
    if (file->eof()) {
      return hydra_t::get();
    } else {
      return hydra_nil::get();
    }
  } else {
    string err = "Non-istream argument provided to end";
    throw err;
  }
}

op_close::op_close() {
  is_fn = true;
  docstring = new hydra_string("Takes an input stream representing a file, and closes it");
  type->arg_list.push_front(new type_nil);
}
hydra_object *op_close::call(hydra_object* alist, runtime& r, lexical_scope &s) {
  list<hydra_object*> arg_list = get_arg_list(alist, r, s);
  if (arg_list.size() != 1) {
    string err = "Invalid number of arguments provided to close"; 
    throw err;
  }

  if (hydra_istream* stream = dynamic_cast<hydra_istream*>(arg_list.front())) {
    ifstream* file = static_cast<ifstream*>(stream->stream);
    file->close();
    return hydra_nil::get();
  } else {
    string err = "Non-istream argument provided to peek";
    throw err;
  }
}
