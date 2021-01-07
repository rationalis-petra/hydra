#include <istream>
#include <sstream>
#include <string>

#include "expressions.hpp"
#include "operations.hpp"

using namespace std;

class matching_paren_exception : public exception {};

ivy_object *read(ivy_object *raw, runtime &r);

ivy_object *to_value(string token) {
  if (token == "nil")
    return new ivy_nil();
  if (token == "t")
    return new ivy_t;
  if (token[0] == '"' && token.back() == '"') {
    token.pop_back();
    ivy_string *str = new ivy_string;
    str->value = &token.c_str()[1];
    return str;
  }
  try {
    int num = stoi(token);
    ivy_num *n = new ivy_num();
    n->val = num;
    return n;
  } catch (invalid_argument &) {
  }
  ivy_symbol *sym = new ivy_symbol();
  sym->symbol = token;
  return sym;
}

ivy_object *to_cons(list<ivy_object *> list) {
  if (list.empty()) {
    return new ivy_nil();
  } else {
    ivy_cons *cns = new ivy_cons();
    cns->car = list.front();
    list.pop_front();
    cns->cdr = to_cons(list);
    return cns;
  }
}

ivy_object *mac_lparen(ivy_object *raw, char c, runtime &r) {
  ivy_istream *is = dynamic_cast<ivy_istream *>(raw);
  // continue to add tokens to list until we hit a ')'
  list<ivy_object *> list;

  while (!is->stream->eof()) {
    c = is->stream->peek();
    switch (c) {
    case ' ':
    case '\n':
    case '\t':
      is->stream->read(&c, 1);
      break;
    case ')': {
      is->stream->read(&c, 1);
      return to_cons(list);
      break;
    }
      break;
    default:
      list.push_back(read(raw, r));
      break;
    }
  }

  throw "( with no matching )!";
}

ivy_object *mac_token(ivy_object *raw, char c, runtime &r) {
  ivy_istream *is = dynamic_cast<ivy_istream *>(raw);
  string token = string("") + c;
  while (!is->stream->eof()) {
    int ch = is->stream->peek();
    if (ch == EOF)
      return to_value(token);

    c = ch;
    switch (c) {
    case ' ':
    case '\n':
    case '\t':
    case '(':
    case ')':
      return to_value(token);
      break;
    default:
      token += c;
    }

    is->stream->read(&c, 1);
  }

  return to_value(token);
}

ivy_object *read(ivy_object *raw, runtime &r) {
  // strings work too!
  ivy_istream *is;
  if (ivy_string *str = dynamic_cast<ivy_string *>(raw)) {
    is = new ivy_istream();
    is->stream = new stringstream(str->value);
  } else if (!(is = dynamic_cast<ivy_istream*>(raw))) {
    string err = "Non-stream type provided to read!";
    throw err;
  }
  char c;

  is->stream->read(&c, 1);
  while (!is->stream->eof()) {

    // if (macro_characters.contains(c));
    // return macro_characters[c]->read(raw, r);
    switch (c) {
      // whitespace characters
    case '\t':
    case '\n':
    case ' ':
      break;

    case '(':
      return mac_lparen(is, c, r);
      break;

    case ')': {
      string err = "error: ')' with no matching '('!";
      throw err;
    }
      break;

    default:
      return mac_token(is, c, r);
      break;
    }

    is->stream->read(&c, 1);
  }

  string err = "Nothing provided to read!";
  throw err;
}

op_read::op_read() { eval_args = true; }
ivy_object *op_read::call(ivy_object *alist, runtime &r) {
  list<ivy_object *> arg_list = get_arg_list(alist, r);
  if (arg_list.size() != 1) {
    string err = "Incorrect number of arguments provided to read";
    throw err;
  }
  return read(arg_list.front(), r);
}
