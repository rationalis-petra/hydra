#include <istream>
#include <sstream>
#include <string>
#include <iostream>

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
    return hydra_nil::get();
  if (token == "t")
    return hydra_t::get();

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
    // if path is empty, we get the root object itself!
    if (path.empty()) {
      return r.root;
    }
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
    return hydra_nil::get();
  } else {
    hydra_cons *cns =
      new hydra_cons(list.front(), (list.pop_front(), to_cons(list)));
    if (hydra_object::node_list.front() != cns) {
      std::cout << "toconsleak" << std::endl;
    }
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
    case '}':
    case ']':
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
  throw "( or [with no matching ) or ]!";
}




hydra_object *mac_token(hydra_istream *is, char c, runtime &r) {
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

hydra_object *mac_string(hydra_istream* is, char c, runtime& r) {
  is->stream->read(&c, 1);
  hydra_string *str = new hydra_string;
  while (!is->stream->eof()) {
    if (c == '"') {
      return str;
    }
    if (c == '\\') {
      is->stream->read(&c, 1);
      str->value += c;
      is->stream->read(&c, 1);
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

    if (r.readtable.find(c) != r.readtable.end()) {
      hydra_cons *cns = new hydra_cons(new hydra_char(c), hydra_nil::get());
      hydra_cons *cns2 = new hydra_cons(is, cns);

      // generate a blank 'dummy' scope
      lexical_scope s;
      return r.readtable[c]->call(cns2, r, s);
    }
    switch (c) {
      // whitespace characters
    case '\t':
    case '\n':
    case ' ':
      break;

    case '{':
    case '[':
    case '(':
      return mac_lparen(is, c, r);
      break;

    case '}':
    case ']':
    case ')': {
      string err = "error: ') or ]' with no matching '( or ['!";
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
  return hydra_nil::get();
}

op_read::op_read() {
  is_fn = true;
  docstring = new hydra_string("Takes a string or input stream, and returns a hydra expression.\n"
                               "Programmable via set-macro-character");
  type->arg_list.push_front(new type_nil);
}
hydra_object *op_read::call(hydra_object *alist, runtime &r, lexical_scope &s) {
  list<hydra_object *> arg_list = get_arg_list(alist, r, s);
  if (arg_list.size() != 1) {
    string err = "Incorrect number of arguments provided to read";
    throw err;
  }
  hydra_object* obj = read(arg_list.front(), r);
  return obj;
}

op_set_mac_char::op_set_mac_char() {
  is_fn = true;
  docstring = new hydra_string("Updates the readtable entry for the character (first argument)\n"
                               "to point to the provided function (second argumetn)");
  type->arg_list.push_front(new type_fn);
  type->arg_list.push_front(new type_nil);
}
hydra_object* op_set_mac_char::call(hydra_object *alist, runtime &r, lexical_scope &s) {
  list<hydra_object *> arg_list = get_arg_list(alist, r, s);
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
