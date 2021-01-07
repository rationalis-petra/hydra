#include <istream>
#include <sstream>
#include <string>

#include "expressions.hpp"
#include "operations.hpp"

using namespace std;

class matching_paren_exception : public exception {};

hydra_object *read(hydra_object *raw, runtime &r);

hydra_object *to_value(string token) {
  if (token == "nil")
    return new hydra_nil();
  if (token == "t")
    return new hydra_t;
  if (token[0] == '"' && token.back() == '"') {
    token.pop_back();
    hydra_string *str = new hydra_string;
    str->value = &token.c_str()[1];
    return str;
  }
  try {
    int num = stoi(token);
    hydra_num *n = new hydra_num();
    n->val = num;
    return n;
  } catch (invalid_argument &) {
  }
  hydra_symbol *sym = new hydra_symbol();
  sym->symbol = token;
  return sym;
}

hydra_object *to_cons(list<hydra_object *> list) {
  if (list.empty()) {
    return new hydra_nil();
  } else {
    hydra_cons *cns = new hydra_cons();
    cns->car = list.front();
    list.pop_front();
    cns->cdr = to_cons(list);
    return cns;
  }
}

hydra_object *mac_lparen(hydra_object *raw, char c, runtime &r) {
  hydra_istream *is = dynamic_cast<hydra_istream *>(raw);
  // continue to add tokens to list until we hit a ')'
  list<hydra_object *> list;

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

hydra_object *mac_token(hydra_object *raw, char c, runtime &r) {
  hydra_istream *is = dynamic_cast<hydra_istream *>(raw);
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

hydra_object *read(hydra_object *raw, runtime &r) {
  // strings work too!
  hydra_istream *is;
  if (hydra_string *str = dynamic_cast<hydra_string *>(raw)) {
    is = new hydra_istream();
    is->stream = new stringstream(str->value);
  } else if (!(is = dynamic_cast<hydra_istream*>(raw))) {
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
hydra_object *op_read::call(hydra_object *alist, runtime &r) {
  list<hydra_object *> arg_list = get_arg_list(alist, r);
  if (arg_list.size() != 1) {
    string err = "Incorrect number of arguments provided to read";
    throw err;
  }
  return read(arg_list.front(), r);
}
