#include <iostream>
#include <istream>
#include <sstream>
#include <string>

#include "expressions.hpp"
#include "operations.hpp"

using std::invalid_argument;
using std::list;
using std::string;
using std::stringstream;

using namespace expr;

Object *read(Object *raw, LocalRuntime &r);

Object *to_value(string token, LocalRuntime &r) {
  // boolean literals
  if (token == "nil")
    return nil::get();
  if (token == "t")
    return t::get();

  // character literal
  if (token[0] == '#') {
    Char *ch;
    if (token.size() == 2) {
      ch = new Char(token[1]);
    } else {
      if (token == "#newline") {
        ch = new Char('\n');
      } else {
        string err = "non-regognized character: " + token;
        throw err;
      }
    }
    return ch;
  }
  try {
    int num = stoi(token);
    return new Integer(num);
  } catch (invalid_argument &) {
  }
  // it is a symbol, first split based on ':'
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
    // if path is empty, we get the root object itself!
    if (path.empty()) {
      return r.r.root;
    }
    Object *obj = r.r.root->intern(path);
    return obj;
    // ':' at beginning is implicit ketword package
  } else if (path.front() == "") {
    path.pop_front();
    path.push_front("keyword");
    Symbol *sym = r.r.root->intern(path);
    sym->value = sym;
    return sym;
    // keyword
  } else {
    Object *obj = r.active_module->intern(path);
    return obj;
  }
}

Object *to_cons(list<Object *> list) {
  if (list.empty()) {
    return nil::get();
  } else {
    Object *car = list.front();
    list.pop_front();
    return new Cons(car, to_cons(list));
  }
}

Object *mac_lcurl(Istream *is, char c, LocalRuntime &r) {
  // continue to add tokens to list until we hit a ')'
  list<Object *> list;

  while (!is->stream->eof()) {
    c = is->stream->peek();
    switch (c) {
    case ' ':
    case '\n':
    case '\t':
      is->stream->read(&c, 1);
      break;
    case '}': {
      is->stream->read(&c, 1);
      return new Cons(op::mk_obj, to_cons(list));
      break;
    } break;
    default:
      list.push_back(read(is, r));
      break;
    }
  }
  throw "read error: { with no matching }!";
}

Object *mac_lsquare(Istream *is, char c, LocalRuntime &r) {
  // continue to add tokens to list until we hit a ')'
  list<Object *> list;

  while (!is->stream->eof()) {
    c = is->stream->peek();
    switch (c) {
    case ' ':
    case '\n':
    case '\t':
      is->stream->read(&c, 1);
      break;
    case ']': {
      is->stream->read(&c, 1);
      return new Cons(op::mk_tuple, to_cons(list));
      break;
    } break;
    default:
      list.push_back(read(is, r));
      break;
    }
  }
  throw "read error: [ with no matching ]!";
}

Object *mac_lparen(Istream *is, char c, LocalRuntime &r) {
  // continue to add tokens to list until we hit a ')'
  list<Object *> list;

  while (!is->stream->eof()) {
    c = is->stream->peek();
    switch (c) {
    case ' ':
    case '\n':
    case '\t':
      is->stream->read(&c, 1);
      break;
    case '}':
    case ')': {
      is->stream->read(&c, 1);
      return to_cons(list);
      break;
    } break;
    default:
      list.push_back(read(is, r));
      break;
    }
  }
  throw "( or [with no matching ) or ]!";
}

Object *mac_token(Istream *is, char c, LocalRuntime &r) {
  string token = string("");
  bool cond = false;
  while (!is->stream->eof()) {
    switch (c) {
    case ' ':
    case '\n':
    case '\t':
    case '{':
    case '}':
    case '(':
    case ')':
    case '[':
    case ']':
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

Object *mac_string(Istream *is, char c, LocalRuntime &r) {
  is->stream->read(&c, 1);
  HString *str = new HString;
  while (!is->stream->eof()) {
    if (c == '"') {
      return str;
    }
    if (c == '\\') {
      is->stream->read(&c, 1);
      str->value += c;
      is->stream->read(&c, 1);
    } else {
      str->value += c;
      is->stream->read(&c, 1);
    }
  }
  string err = "non-terminated string";
  throw err;
}

Object *read(Object *raw, LocalRuntime &r) {
  // strings work too!
  Istream *is;
  IOstream *ios = nullptr;
  if (HString *str = dynamic_cast<HString *>(raw)) {
    is = new Istream();
    is->stream = new stringstream(str->value);
  } else if (!(is = dynamic_cast<Istream *>(raw)) &&
             !(ios = dynamic_cast<IOstream *>(raw))) {
    string err = "Non-stream type provided to read!";
    throw err;
  }
  char c = '\0';

  if (ios) {
    is = new Istream;
    is->stream = ios->stream;
  }
  is->stream->read(&c, 1);
  while (!is->stream->eof()) {

    if (r.r.readtable.find(c) != r.r.readtable.end()) {
      Cons *cns = new Cons(new Char(c), nil::get());
      Cons *cns2 = new Cons(is, cns);

      // generate a blank 'dummy' scope
      LexicalScope s;
      list<Object *> arg_list = r.r.readtable[c]->get_arg_list(cns2, r, s);
      return r.r.readtable[c]->call(arg_list, r, s);
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
    case '[':
      return mac_lsquare(is, c, r);
      break;
    case '{':
      return mac_lcurl(is, c, r);
      break;

    case '}':
    case ']':
    case ')': {
      string err = "error: ') or ]' with no matching '( or ['!";
      throw err;
    } break;
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
  return nil::get();
}

Object *op_read(list<Object *> arg_list, LocalRuntime &r, LexicalScope &s) {

  if (arg_list.size() != 1) {
    string err = "Incorrect number of arguments provided to read";
    throw err;
  }
  Object *obj = read(arg_list.front(), r);
  return obj;
}

Object *op_set_mac_char(list<Object *> arg_list, LocalRuntime &r,
                        LexicalScope &s) {

  if (arg_list.size() != 2) {
    string err =
        "Incorrect number of arguments provided to set-macro-character";
    throw err;
  }
  Char *character = dynamic_cast<Char *>(arg_list.front());
  Operator *function = dynamic_cast<Operator *>(arg_list.back());
  if (!character) {
    string err = "set-macro-character expects character as first argument";
    throw err;
  }
  if (!function) {
    string err = "set-macro-character expects function as second argument";
    throw err;
  }
  r.r.readtable[character->value] = function;
  return function;
}

Operator *op::read;
Operator *op::set_mac_char;

void op::initialize_read() {
  op::read = new InbuiltOperator("read",
      "Takes a string or input stream, and returns a hydra expression.\n"
      "Programmable via set-macro-character",
      // type (Union Input-stream String)
      op_read, type::Fn::with_args({new type::Any}), true);

  op::set_mac_char = new InbuiltOperator("set-macro-character",
      "Updates the readtable entry for the character (first argument)\n"
      "to point to the provided function (second argumetn)",
      op_set_mac_char, type::Fn::with_args({type::character_type, new type::Fn}),
      true);
}
