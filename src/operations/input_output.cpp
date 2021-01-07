#include <iostream>
#include <fstream>
#include <string>
#include <list>

#include "expressions.hpp"
#include "operations.hpp"

using namespace std;

op_print::op_print() { eval_args = true; }
ivy_object *op_print::call(ivy_object *alist, runtime &r) {
  list<ivy_object *> arg_list = get_arg_list(alist, r);
  if (arg_list.size() != 1) {
    throw "Invalid number of arguments to print: print takes 1 argument";
  }
  cout << arg_list.front()->to_string() << endl;
  return arg_list.front();
}

op_open_file::op_open_file() { eval_args = true; }
ivy_object *op_open_file::call(ivy_object *alist, runtime &r) {
  list<ivy_object *> arg_list = get_arg_list(alist, r);
  if (arg_list.size() != 1) {
    throw "Invalid number of arguments to open-file: print takes 1 argument";
  }

  ivy_object* filename = arg_list.front();
  try {
    ivy_string* strfilename = dynamic_cast<ivy_string*>(filename);
    ivy_istream* out = new ivy_istream();
    out->stream = new ifstream(strfilename->value);
    return out;
  } catch (bad_cast&) {
    string err = "Inavalid argument to open-file: " + filename->to_string();
    throw err;
  }
}

op_next::op_next() { eval_args = true; }
ivy_object *op_next::call(ivy_object* alist, runtime& r) {
  list<ivy_object*> arg_list = get_arg_list(alist, r);
  if (arg_list.size() != 1) {
    string err = "Invalid number of arguments provided to next"; 
  }

  if (ivy_istream* stream = dynamic_cast<ivy_istream*>(arg_list.front())) {
    ivy_char* c = new ivy_char;
    stream->stream->read(&c->value, 1);
    return c;
  } else {
    string err = "Non-istream argument provided to peek";
    throw err;
  }
}

op_peek::op_peek() { eval_args = true; }
ivy_object *op_peek::call(ivy_object* alist, runtime& r) {
  list<ivy_object*> arg_list = get_arg_list(alist, r);
  if (arg_list.size() != 1) {
    string err = "Invalid number of arguments provided to peek"; 
    throw err;
  }
  if (ivy_istream* stream = dynamic_cast<ivy_istream*>(arg_list.front())) {
    ivy_char* c = new ivy_char;
    c->value = stream->stream->peek();
    return c;
  } else {
    string err = "Non-istream argument provided to peek";
    throw err;
  }
}

op_close::op_close() { eval_args = true; }
ivy_object *op_close::call(ivy_object* alist, runtime& r) {
  list<ivy_object*> arg_list = get_arg_list(alist, r);
  if (arg_list.size() != 1) {
    string err = "Invalid number of arguments provided to close"; 
    throw err;
  }

  if (ivy_istream* stream = dynamic_cast<ivy_istream*>(arg_list.front())) {
    ifstream* file = static_cast<ifstream*>(stream->stream);
    file->close();
    return new ivy_nil;
  } else {
    string err = "Non-istream argument provided to peek";
    throw err;
  }
}
