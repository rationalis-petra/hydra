#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <utility>
#include <variant>

#include "expressions.hpp"
#include "operations.hpp"
using namespace std;

extern string lang;

using namespace expr;
Module *expr::language_module;
Module *expr::keyword_module;
Module *expr::core_module;
GenericFn *expr::equal_operator;
GenericFn *expr::gn_to_string;

using type::hydra_cast;

vector<pair<string, Object *>> inbuilts;

vector<pair<string, Object *>> dev;
vector<pair<string, Object *>> core;
vector<pair<string, Object *>> foreign;
vector<pair<string, Object *>> io;
vector<pair<string, Object *>> concurrent;
// net
// concurrent / parallel

Runtime& g = Object::r;
LocalRuntime r(g);

Object *read(Istream *istm);
void make_modules();

int main(int argc, char **argv) {
  Integer::parent = new Parent("Integer parent");
  HString::parent = new Parent("String parent");
  Cons::parent = new Parent("Cons parent");
  Tuple::parent = new Parent("Tuple parent");
  Char::parent = new Parent("Char parent");
  Union::parent = new Parent("Union parent");

  g.root = new Module("");
  language_module = new Module("hydra");
  Symbol *sym = g.root->intern("hydra");
  sym->value = language_module;
  r.active_module = language_module;
  sym = g.root->intern("keyword");
  sym->value = new Module("keyword");
  keyword_module = (Module*) sym->value;

  // SPECIAL: type initialization
  op::initialize_type_ops();
  type::initialize_types();
  op::type_type_ops();


  op::mk_arithmetic();
  op::initialize_user_obj();
  op::initialize_cons();
  op::initialize_string();
  op::initialize_vector();
  op::initialize_tuple();
  op::initialize_dev();
  op::initialize_conditions();
  op::initialize_logic();
  op::initialize_language();
  op::initialize_module();
  op::initialize_mkoperator();
  op::initialize_io();
  op::initialize_foreign();
  op::initialize_concurrency();

  // we place read last because it uses other operations
  op::initialize_read();


  make_modules();

  // Object::r = &r;

  // arithmetic

  Module *mod = language_module;
  for (auto p : inbuilts) {
    Symbol *sym = mod->intern(p.first);
    sym->value = p.second;
  }

  vector<pair<string, vector<pair<string, Object *>>>> moddefs = {
      make_pair("core", core),
      make_pair("io", io),
      make_pair("foreign", foreign),
      make_pair("dev", dev),
      make_pair("concurrent", concurrent)};

  for (auto m : moddefs) {
    mod = hydra_cast<Module>(
        hydra_cast<Symbol>(language_module->get(m.first))->value);
    for (auto p : m.second) {
      Symbol *sym = mod->intern(p.first);
      mod->exported_symbols[p.first] = sym;
      sym->value = p.second;
    }
  }

  Istream *stm = new Istream();
  stm->stream = &cin;
  sym =
      hydra_cast<Module>(language_module->intern("io")->value)->intern("+cin+");
  sym->value = stm;

  string in;
  Istream *prog = new Istream();
  prog->stream = new stringstream(lang);
  Object *ast;
  Object *out;

  // if argc > 1, assume they are file names
  int count = argc;
  while (count > 0) {
    try {
      while (!prog->stream->eof()) {
        LexicalScope s;
        ast = read(prog);
        out = ast->eval(r, s);
      }
    } catch (string e) {
      cout << e << endl;
    } catch (const char *err) {
      cout << err << endl;
    }
    count--;
    if (count != 0) {
      delete prog->stream;
      prog->stream = new stringstream("(eval (load \"" +
                                      string(argv[argc - count]) + "\"))");
    }
  }

  while (true) {
    try {
      cout << r.active_module->name << "> ";
      LexicalScope s;
      ast = read(stm);
      Object::roots.insert(ast);
      out = ast->eval(r, s);
      cout << "* ";
      op::print->call({out}, r, s);
      cout << endl;
      Object::roots.remove(ast);
      Object::collect_garbage(r);
    } catch (hydra_exception *e) {
      cout << "Top-level exception thrown!" << endl;
      cout << "Object: " << e->obj << endl;
    } catch (string e) {
      cout << e << endl;
    } catch (const char *err) {
      cout << err << endl;
    }

    getline(cin, in);
  }

  cout << Object::node_list.size() << endl;
  for (Object *obj : Object::node_list) {
    cout << obj << endl;
    delete obj;
  }
  return 0;
}

void make_modules() {
  inbuilts = {// other modules
              make_pair("core", new Module("core")),
              make_pair("io", new Module("io")),
              make_pair("foreign", new Module("foreign")),
              make_pair("concurrent", new Module("concurrent")),
              make_pair("dev", new Module("dev"))};

  dev = {make_pair("doc", op::describe),
    make_pair("macro-expand", op::macexpand),
    make_pair("time", op::time)};

  Operator* op_int_eq = op::equal->functions.front();
  op::equal->add(op::obj_eq);
  op::equal->add(op_int_eq);
  op::equal->add(op::char_eq);
  op::equal->add(op::tuple_eq);
  op::equal->add(op::cons_eq);
  op::equal->add(op::vec_eq);
  op::equal->add(op::str_eq);
  op::equal->add(op::type_eq);
  equal_operator = op::equal;

  GenericFn *gn_elt = new GenericFn;
  gn_elt->is_fn = true;
  gn_elt->type->rest_type = new type::Any;
  gn_elt->add(op::vec_elt);
  gn_elt->add(op::str_elt);
  gn_elt->add(op::tuple_elt);

  GenericFn *gn_len = new GenericFn;
  gn_len->is_fn = true;
  gn_len->type->rest_type = new type::Any;
  gn_len->add(op::vec_len);
  // gn_elt->add(op::str_elt);

  GenericFn *gn_concat = new GenericFn;
  gn_concat->is_fn = true;
  gn_concat->type->rest_type = new type::Any;
  gn_concat->add(op::str_cat);
  gn_concat->add(op::vec_cat);

  op::greater->add(op::str_gr);

  GenericFn *gn_get = new GenericFn;
  gn_get->is_fn = true;
  gn_get->type->arg_list.push_back(new type::Any);
  gn_get->type->arg_list.push_back(new type::Any);
  gn_get->add(op::obj_get);
  gn_get->add(op::get);

  GenericFn *gn_set = new GenericFn;
  gn_set->is_fn = true;
  gn_set->type->arg_list.push_back(new type::Any);
  gn_set->type->arg_list.push_back(new type::Any);
  gn_set->type->arg_list.push_back(new type::Any);
  gn_set->add(op::obj_set);
  gn_set->add(op::vec_set);

  gn_to_string = new GenericFn;
  gn_to_string->is_fn = true;
  gn_to_string->type->arg_list.push_back(new type::Any);
  gn_to_string->type->return_type = type::string_type;
  gn_to_string->add(op::to_str);

  core = {

    // arithmetic
    make_pair("+", op::plus),
    make_pair("-", op::minus),
    make_pair("*", op::multiply),
    make_pair("/", op::divide),
    make_pair(">", op::greater),
    // data
    make_pair("object", op::mk_obj),
    make_pair("tuple", op::mk_tuple),
    make_pair("union", op::mk_union),
    make_pair("vector", op::mk_vec),
    make_pair("cons", op::mk_cons),
    make_pair("car", op::car),
    make_pair("cdr", op::cdr),
    make_pair("elt", gn_elt),
    make_pair("concat", gn_concat),
    make_pair("len", gn_len),

    make_pair("to-string", gn_to_string),

    make_pair("set-invoker", op::set_invoker),
    make_pair("get-invoker", op::get_invoker),
    make_pair("set", gn_set),
    make_pair("get", gn_get),
    make_pair("clone", op::clone),

    // streams
    make_pair("peek", op::peek),
    make_pair("put", op::put),
    make_pair("next", op::next),
    make_pair("end?", op::endp),
    make_pair("eval", op::eval),
    make_pair("read", op::read),
    make_pair("set-macro-character", op::set_mac_char),
    // logic
    make_pair("=", op::equal),
    make_pair("or", op::do_or),
    make_pair("and", op::do_and),
    make_pair("not", op::do_not),

    // symbols

    make_pair("defined?", op::definedp),
    make_pair("bind", op::bind),
    make_pair("unbind", op::unbind),
    make_pair("lock", op::lock),
    make_pair("unlock", op::unlock),

    // language/control-flow
    make_pair("if", op::do_if),
    make_pair("while", op::do_while),
    make_pair("fn", op::fn),
    make_pair("gen", op::genfn),
    make_pair("mac", op::mac),
    make_pair("quote", op::quote),
    make_pair("progn", op::progn),
    make_pair("add-fn", op::addfn),
    make_pair("exit", op::exit),

    make_pair("module", op::mk_module),
    make_pair("symbol", op::mk_symbol),
    make_pair("in-module", op::in_module),
    make_pair("current-module", op::get_module),
    make_pair("get-symbols", op::get_symbols),
    make_pair("export", op::do_export),
    make_pair("insert", op::insert),
    make_pair("intern", op::intern),
    make_pair("remove", op::remove),

    // CONDiTIONS
    make_pair("signal-condition", op::signal_condition),
    make_pair("handler-case", op::handler_catch),
    make_pair("handler-bind", op::handler_bind),
    make_pair("with-restart", op::add_restart),
    make_pair("get-restarts", op::get_restarts),

    make_pair("Integer-Parent", Integer::parent),

    // types
    make_pair("Is", op::mk_is),
    make_pair("Derives", op::mk_derives),
    //make_pair("Object", new type::Object),
    make_pair("Any", new type::Any),
    make_pair("Integer", type::integer_type),
    make_pair("Nil", type::nil_type),
    make_pair("String", type::string_type),
    make_pair("Char", type::character_type),
    make_pair("Tuple", new type::Tuple),
    make_pair("Vector", new type::Vector),
    make_pair("Type", new type::MetaType),
    make_pair("Module", new type::Module),
    make_pair("Symbol", new type::Symbol),
    make_pair("List", new type::List),
    make_pair("IOStream", new type::IOStream),
    make_pair("IStream", new type::Istream),
    make_pair("OStream", new type::Ostream),
    make_pair("Cons", new type::Cons),
    // Mac
    // Op
    make_pair("Fn", new type::Fn),
    make_pair("Gen", new type::GenFn),

    make_pair("type?", op::typep),
    //make_pair("type", op::mk_type),
    make_pair("subtype", op::subtype)
  };

  foreign = {// ffi
             make_pair("load-c-library", op::foreign_lib),
             make_pair("get-c-symbol", op::foreign_sym),
             make_pair("internalize", op::internalize)};

  io = {// io
        make_pair("print", op::print),
        make_pair("open-file", op::open_file),
        make_pair("close-file", op::close)};

  concurrent = {
    make_pair("thread", op::thread)};
}

Object *read(Istream *istm) {
  // blank scope
  LexicalScope scope;
  Symbol *cores = language_module->intern("core");
  Module *corem = hydra_cast<Module>(cores->eval(r, scope));
  core_module = corem;

  Symbol* s = corem->intern("read");
  Object *v = s->eval(r, scope);
  Operator *op = hydra_cast<Operator>(v);
  return op->call({istm}, r, scope);
}
