#include <iostream>
#include <fstream>
#include <string>
#include <list>

#include "expressions.hpp"
#include "operations.hpp"

using std::list;
using std::string;
using std::ifstream;
using std::istream;
using std::cout;
using std::endl;

op_print::op_print() {
  is_fn = true;
  docstring = new hydra_string("Prints a string representation of the provided argument to\n"
                               "the standard output stream");
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
}
hydra_object *op_open_file::call(hydra_object *alist, runtime &r, lexical_scope& s) {
  list<hydra_object *> arg_list = get_arg_list(alist, r, s);
  if (arg_list.size() != 1) {
    throw "Invalid number of arguments to open-file: print takes 1 argument";
  }

  hydra_object *filename = arg_list.front();
  if (hydra_string *strfilename = dynamic_cast<hydra_string *>(filename)) {
    hydra_istream *out = new hydra_istream();
    out->stream = new ifstream(strfilename->value);
    return out;
  } else {
    string err = "Inavalid argument to open-file: " + filename->to_string();
    throw err;
  }
}

op_next::op_next() {
  is_fn = true;
  docstring = new hydra_string("Takes an input stream, advances it one character\n"
                               "and returns the character at the 'current' position");
}
hydra_object *op_next::call(hydra_object* alist, runtime& r, lexical_scope& s) {
  list<hydra_object*> arg_list = get_arg_list(alist, r, s);
  if (arg_list.size() != 1) {
    string err = "Invalid number of arguments provided to next"; 
  }

  if (hydra_istream* stream = dynamic_cast<hydra_istream*>(arg_list.front())) {
    char ch;
    stream->stream->read(&ch, 1);
    return new hydra_char(ch);
  } else {
    string err = "Non-istream argument provided to peek";
    throw err;
  }
}

op_peek::op_peek() {
  is_fn = true;
  docstring = new hydra_string("Takes an input stream, and peeks at the next character\n"
                               "but does not advance the input stream");
}
hydra_object *op_peek::call(hydra_object* alist, runtime& r, lexical_scope &s) {
  list<hydra_object*> arg_list = get_arg_list(alist, r, s);
  if (arg_list.size() != 1) {
    string err = "Invalid number of arguments provided to peek"; 
    throw err;
  }
  if (hydra_istream* stream = dynamic_cast<hydra_istream*>(arg_list.front())) {
    hydra_char* c = new hydra_char;
    c->value = stream->stream->peek();
    return c;
  } else {
    string err = "Non-istream argument provided to peek";
    throw err;
  }
}

op_endp::op_endp() {
  is_fn = true;
  docstring = new hydra_string("Returns t if a given input stream has reached the\n"
                               "end of the file, and nil otherwise");
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
