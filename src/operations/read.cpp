#include <istream>
#include <sstream>
#include <string>

#include "expressions.hpp"
#include "operations.hpp"

using std::list;
using std::string;
using std::invalid_argument;
using std::stringstream;

hydra_object *read(hydra_object *raw, runtime &r);

hydra_object *to_value(string token, runtime & r) {
  // boolean literals
  if (token == "nil")
    return new hydra_nil();
  if (token == "t")
    return new hydra_t;

  // character literal
  if (token[0] == '#') {
    hydra_char* ch = new hydra_char;
    if (token.size() == 2) {
      ch->value = token[1];
    } else {
      if (token == "#newline") {
        ch->value = '\n';
      }
      else {
        string err = "non-regognized character: " + token;
        throw err;
      }
    }
    return ch;
  }
  try {
    int num = stoi(token);
    return new hydra_num(num);
  } catch (invalid_argument &) {
  }
  //it is a symbol, first split based on ':'
  list<string> path;
  string current = "";
  for (char c : token) {
    if (c == ':') {
      path.push_back(current);
      current = "";
    } else {
      current += c;
    }
  }
  path.push_back(current);

  // & = root package
  if (path.front() == "&") {
    path.pop_front();
    hydra_object* obj = r.root->intern(path);
    return obj;
    // ':' at beginning is implicit ketword package
  } else if (path.front() == "") {
    path.pop_front();
    path.push_front("keyword");
    hydra_symbol* obj = r.root->intern(path);
    obj->value = obj;
    return obj;
    // keyword
  } else {
    hydra_object *obj = r.active_module->intern(path);
    return obj;
  }
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

hydra_object *mac_lparen(hydra_istream *is, char c, runtime &r) {
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
      list.push_back(read(is, r));
      break;
    }
  }

  throw "( with no matching )!";
}

hydra_object *mac_token(hydra_istream *is, char c, runtime &r) {
  string token = string("");
  bool cond = false;
  while (!is->stream->eof()) {
    switch (c) {
    case ' ':
    case '\n':
    case '\t':
    case '(':
    case ')':
      return to_value(token, r);
      break;
      // escape character!
    case '\\': // treat the next character as normal
      token += is->stream->peek();
      is->stream->read(&c, 1);
      break;
      // the hash is used for various literals

    default:
      token += c;
      break;
    }

    if (cond) {
      is->stream->read(&c, 1);
    }
    cond = true;
    int ch = is->stream->peek();
    if (ch == EOF)
      return to_value(token, r);
    c = ch;
  }

  return to_value(token, r);
}

hydra_object *mac_string(hydra_istream* is, char c, runtime& r) {
  is->stream->read(&c, 1);
  hydra_string *str = new hydra_string;
  while (!is->stream->eof()) {
    if (c == '"') {
      return str;
    }
    else {
      str->value += c;
      is->stream->read(&c, 1);
    }
  }
  string err = "non-terminated string";
  throw err;
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

    if (r.readtable.find(c) != r.readtable.end()){
      hydra_cons* cns = new hydra_cons;
      cns->car = new hydra_char(c);
      cns->cdr = new hydra_nil;
      hydra_cons* cns2 = new hydra_cons;
      cns2->car = is;
      cns2->cdr = cns;
      return r.readtable[c]->call(cns2, r);
    }
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
    case '"': {
      return mac_string(is, c, r);
    }

    default:
      return mac_token(is, c, r);
      break;
    }

    is->stream->read(&c, 1);
  }

  // todo: more advanced read options!
  return new hydra_nil;
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

op_set_mac_char::op_set_mac_char() { eval_args = true; }
hydra_object* op_set_mac_char::call(hydra_object *alist, runtime &r) {
  list<hydra_object *> arg_list = get_arg_list(alist, r);
  if (arg_list.size() != 2) {
    string err = "Incorrect number of arguments provided to set-macro-character";
    throw err;
  }
  hydra_char* character = dynamic_cast<hydra_char *>(arg_list.front());
  hydra_oper* function = dynamic_cast<hydra_oper *>(arg_list.back());
  if (!character) {
    string err = "set-macro-character expects character as first argument";
    throw err;
  }
  if (!function) {
    string err = "set-macro-character expects function as second argument";
    throw err;
  }
  r.readtable[character->value] = function;
  return function;
}
