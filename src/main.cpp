#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <utility>
#include <variant>

#include "expressions.hpp"
#include "operations.hpp"
#include "utils.hpp"
#include "vals.hpp"

using namespace std;

using namespace expr;

extern string lang;

Module *expr::language_module = nullptr;
Module *expr::keyword_module = nullptr;
Module *expr::core_module = nullptr;
GenericFn *expr::equal_operator = nullptr;
GenericFn *expr::gn_to_string = nullptr;

using type::hydra_cast;

vector<pair<string, Object *>> inbuilts;

vector<pair<string, Object *>> dev;
vector<pair<string, Object *>> core;
vector<pair<string, Object *>> foreign;
vector<pair<string, Object *>> io;
vector<pair<string, Object *>> concurrent;
vector<pair<string, Object *>> network;
vector<pair<string, Object *>> sys_module;

// net
// concurrent / parallel

interp::Runtime g;
interp::LocalRuntime r(g);

Object *read(Istream *istm);
void make_modules();

int main(int argc, char **argv) {
  Object::collector = new interp::STWCollector(g);

  // SETUP PARENTS
  // As hydra is prototype-based, objects of each "type" (integer, string, ...)
  // must notionally inherit from some parent object - the "string-parent" or
  // "integer parent", etc. We create them here.

  // Common-bechaviour and default-behaviour are two special cases:
  // common-behaviour acts a parent to almost all objects
  Object::common_behaviour = new Parent("Common Behaviour");
  Object::default_behaviour = new Parent("Default Behaviour");

  // Here are the rest of the "normal" types
  Number::parent = new Parent("number-parent");
  Complex::parent = new Parent("complex-parent");
  Real::parent = new Parent("real-parent");
  Integer::parent = new Parent("integer-parent");
  Float::parent = new Parent("float-parent");

  HString::parent = new Parent("string-parent");
  Mutex::parent = new Parent("mutex-parent");
  Cons::parent = new Parent("cons-parent");
  Tuple::parent = new Parent("tuple-parent");
  Char::parent = new Parent("char-parent");
  Union::parent = new Parent("union-parent");
  Mirror::parent = new Parent("mirror-parent");
  Module::parent = new Parent("module-parent");
  Symbol::parent = new Parent("symbol-parent");

  Istream::parent = new Parent("istream-parent");
  Ostream::parent = new Parent("ostream-parent");
  IOstream::parent = new Parent("iostream-parent");
  Socket::parent = new Parent("socket-parent");
  Acceptor::parent = new Parent("acceptor-parent");


  // SETUP MODULES
  // We need to specially setup the keyword module because 
  // we don't have the "parent" symbol yet!!
  // so we need to do some special out-of sequence construction to
  // avoid "parent" attempting to intern itself in an endless loop...
  keyword_module = new Module();
  keyword_module->name = "keyword";
  {
    // Symbols are descended from the Symbol-Parent object, which lives
    // in the slot with symbol 'parent'. Hence, we first need to create
    // the parent symbol, then assign it's slot with symbol parent (i.e. itself)
    // to the Symbol-Parent object.
    Symbol *parent_sym = Symbol::symbol_no_parent("parent");
    keyword_module->symbols["parent"] = parent_sym;
    parent_sym->slots[parent_sym] = Symbol::parent;
    parent_sym->parents.insert(parent_sym);
  }

  // Construct the root module & place keyword within it
  g.root = new Module();
  Symbol* sym = g.root->intern("keyword");
  sym->value = keyword_module;

  // now, we can set both the root and keyword modules to have their
  // parent be the Module-Parent object
  g.root->set_parent();
  keyword_module->set_parent();

  // we also setup some parent relations for types
  Object::default_behaviour->slots[get_keyword("parent")] = Object::common_behaviour;


  language_module = new Module("hydra");
  sym = g.root->intern("hydra");
  sym->value = language_module;
  r.active_module = language_module;


  // SETUP type hierarchy
  IOstream::parent->set_parent("istream", Istream::parent);
  IOstream::parent->set_parent("ostream", Ostream::parent);
  Socket::parent->set_parent("parent", IOstream::parent);

  Number::parent->set_parent("parent", Object::common_behaviour);
  Complex::parent->set_parent("parent", Number::parent);
  Real::parent->set_parent("parent", Number::parent);
  Integer::parent->set_parent("parent", Real::parent);
  Float::parent->set_parent("parent", Real::parent);

  // SETUP Operators
  // we have operators on types, which is required to create some types,
  // but operators need to be annotated with types. Hence, the first three
  // functions here are special: initialize_type_ops initializes the operators
  // but does not add type annotations. Once that is done, we can create (initialze)
  // the types, then add annotations to the operatos which operate on types...
  op::initialize_type_ops();
  type::initialize_types();
  op::type_type_ops();

  // this are more simple - simply assigning the operatos like '+' and
  // 'to-string' to global variables 
  // these two define generic functions that are inserted into by subsequent
  // initializations
  op::initialize_logic();
  op::initialize_data();

  // these define "singleton" functions (generic functions with only one value)
  op::initialize_arithmetic();
  op::initialize_mirror();
  op::initialize_user_obj();
  op::initialize_cons();
  op::initialize_string();
  op::initialize_vector();
  op::initialize_tuple();
  op::initialize_dev();
  op::initialize_conditions();
  op::initialize_language();
  op::initialize_module();
  op::initialize_mkoperator();
  op::initialize_foreign();
  op::initialize_concurrency();
  op::initialize_system();

  // io contains generics needed by other io  
  // operators, such as netowrking
  op::initialize_io();
  op::initialize_network();

  // we place read last because it uses other operations
  op::initialize_read();

  // initialize global values so they can be palced in modules
  {
    std::list<Object*> args;
    for (int i = 1; i < argc; i++) {
      args.push_back(new HString(argv[i]));
    }
    vals::args = list_to_cons(args);
  }

  make_modules();

  // arithmetic

  for (auto p : inbuilts) {
    Symbol *sym = language_module->intern(p.first);
    sym->value = p.second;
  }

  vector<pair<string, vector<pair<string, Object *>>>> moddefs = {
      make_pair("core", core),
      make_pair("io", io),
      make_pair("foreign", foreign),
      make_pair("dev", dev),
      make_pair("concurrent", concurrent),
      make_pair("network", network),
      // system already declared in c++, so we use sys_module
      make_pair("system", sys_module)};

  for (auto m : moddefs) {
    Module* mod = dynamic_cast<Module*>(
        dynamic_cast<Symbol*>(language_module->get(m.first))->value);
    for (auto p : m.second) {
      Symbol *sym = mod->intern(p.first);
      mod->exported_symbols[p.first] = sym;
      sym->value = p.second;
    }
  }

  Istream *hydra_cin = new Istream();
  hydra_cin->stream = &cin;
  sym =
      hydra_cast<Module>(language_module->intern("io")->value)->intern("+cin+");
  sym->value = hydra_cin;
  Ostream* hydra_cout = new Ostream();
  hydra_cout->stream = &cout;
  sym =
      hydra_cast<Module>(language_module->intern("io")->value)->intern("+cout+");
  sym->value = hydra_cout;

  string in;
  Istream *prog = new Istream();
  prog->stream = new stringstream(lang);
  Object *ast;
  Object *out;

  // execute string in lang.cpp
  try {
    while (!prog->stream->eof()) {
      interp::LexicalScope s;
      ast = read(prog);
      out = ast->eval(r, s);
    }
  } catch (string e) {
    cout << e << endl;
  } catch (const char *err) {
    cout << err << endl;
  }

  // if argc > 1, assume that the first argument
  // is a file name (the rest are placed in system:args)
  if (argc > 1) {
    delete prog->stream;
    prog->stream = new stringstream("(eval (load \"" +
                                    string(argv[1]) + "\"))");
    try {
      while (!prog->stream->eof()) {
        interp::LexicalScope s;
        ast = read(prog);
        out = ast->eval(r, s);
      }
    } catch (string e) {
      cout << e << endl;
    } catch (const char *err) {
      cout << err << endl;
    }
  }
  delete prog->stream;

  while (true) {
    interp::LexicalScope s;
    try {
      cout << r.active_module->name << "> ";
      ast = read(hydra_cin);
      Object::collector->insert_root(ast);
      out = ast->eval(r, s);
      cout << "* ";
      op::print->call({out}, r, s);
      cout << endl;
      Object::collector->remove_root(ast);
      Object::collector->collect_garbage(r);
    } catch (hydra_exception *e) {
      cout << "Caught exception at top level!" << endl;
      cout << hydra_to_string(e->obj, r, s) << endl;
    } catch (string e) {
      cout << e << endl;
    } catch (const char *err) {
      cout << err << endl;
    }

    getline(cin, in);
  }
  return 0;
}

void make_modules() {
  inbuilts = {// other modules
              make_pair("core", new Module("core")),
              make_pair("io", new Module("io")),
              make_pair("foreign", new Module("foreign")),
              make_pair("concurrent", new Module("concurrent")),
              make_pair("system", new Module("system")),
              make_pair("dev", new Module("dev")),
              make_pair("network", new Module("network"))};

  dev = {make_pair("doc", op::describe),
    make_pair("macro-expand", op::macexpand),
    make_pair("time", op::time)};

  op::bin_equal->add(op::type_eq);
  equal_operator = op::equal;

  gn_to_string = new GenericFn;
  gn_to_string->is_fn = true;
  gn_to_string->type->arg_list.push_back(new type::Any);
  gn_to_string->type->return_type = type::string_type;
  gn_to_string->add(op::to_str);

  core = {
    // arithmetic
    make_pair("binary+", op::bin_plus),
    make_pair("binary-", op::bin_minus),
    make_pair("binary*", op::bin_multiply),
    make_pair("binary/", op::bin_divide),
    make_pair("binary>", op::bin_greater),
    make_pair("sqrt", op::sqrt),

    make_pair("+", op::plus),
    make_pair("-", op::minus),
    make_pair("*", op::multiply),
    make_pair("/", op::divide),
    make_pair(">", op::greater),


    // data
    make_pair("get", op::get),
    make_pair("set", op::set),
    make_pair("concat", op::cat),
    make_pair("len", op::len),
    // constructors
    make_pair("cmplx", op::mk_cmplx),
    make_pair("tuple", op::mk_tuple),
    make_pair("union", op::mk_union),
    make_pair("vector", op::mk_vec),
    make_pair("cons", op::mk_cons),
    // access
    make_pair("car", op::car),
    make_pair("cdr", op::cdr),

    make_pair("to-string", gn_to_string),
    
    // OBJECTS & REFLECTION
    make_pair("object", op::mk_obj),
    make_pair("reflect", op::get_mirror),
    make_pair("get-slots", op::get_slots),
    make_pair("get-slot", op::get_slot_val),
    make_pair("set-slot", op::set_slot_val),
    make_pair("remove-slot", op::delete_slot),
    make_pair("get-meta", op::get_meta),
    make_pair("get-metaslot", op::get_meta_val),
    make_pair("set-metaslot", op::set_meta_val),
    make_pair("remove-metaslot", op::delete_meta),

    make_pair("set-invoker", op::set_invoker),
    make_pair("get-invoker", op::get_invoker),
    make_pair("clone", op::clone),

    // streams
    make_pair("write", op::write),
    make_pair("peek", op::peek),
    make_pair("put", op::put),
    make_pair("next", op::next),
    make_pair("end?", op::endp),
    make_pair("eval", op::eval),
    make_pair("read", op::read),
    make_pair("set-macro-character", op::set_mac_char),
    // logic
    make_pair("=", op::equal),
    make_pair("binary=", op::bin_equal),
    make_pair("or", op::do_or),
    make_pair("and", op::do_and),
    make_pair("not", op::do_not),

    // symbols

    make_pair("defined?", op::definedp),
    make_pair("bind", op::bind),
    make_pair("unbind", op::unbind),
    make_pair("mutable", op::mk_mutable),
    make_pair("const", op::mk_const),

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

    // CONDITIONS
    make_pair("signal-condition", op::signal_condition),
    make_pair("handler-case", op::handler_catch),
    make_pair("handler-bind", op::handler_bind),
    make_pair("with-restart", op::add_restart),
    make_pair("get-restarts", op::get_restarts),


    // TYPES
    make_pair("Is", op::mk_is),
    make_pair("Derives", op::mk_derives),
    make_pair("Any", new type::Any),
    make_pair("Nil", type::nil_type),
    make_pair("String", type::string_type),
    make_pair("Char", type::character_type),
    make_pair("Tuple", new type::Tuple),
    make_pair("Vector", new type::Vector),
    make_pair("Type", new type::MetaType),
    make_pair("Module", type::module_type),
    make_pair("Symbol", type::symbol_type),
    make_pair("List", new type::List),
    make_pair("Cons", new type::Cons),

    make_pair("Number", type::number_type),
    make_pair("Complex", type::complex_type),
    make_pair("Real", type::real_type),
    make_pair("Integer", type::integer_type),
    make_pair("Float", type::float_type),

    make_pair("IOStream", type::iostream_type),
    make_pair("IStream", type::istream_type),
    make_pair("OStream", type::ostream_type),
    make_pair("Socket", type::socket_type),
    // MAC
    // OP
    make_pair("Fn", new type::Fn),
    make_pair("Gen", new type::GenFn),

    make_pair("type?", op::typep),
    //make_pair("type", op::mk_type),
    make_pair("subtype?", op::subtype)
  };

  foreign = {// ffi
             make_pair("load-c-library", op::foreign_lib),
             make_pair("get-c-symbol", op::foreign_sym),
             make_pair("internalize", op::internalize)};

  io = {// io
        make_pair("print", op::print),
        make_pair("open-file", op::open_file),
        make_pair("close", op::close)};

  concurrent = {
    make_pair("thread", op::thread),
    make_pair("mutex", op::mk_mutex),
    make_pair("lock", op::lock_mutex),
    make_pair("unlock", op::unlock_mutex)};

  network = {
    make_pair("tcp-socket", op::mk_tcp_socket),
    make_pair("tcp-acceptor", op::mk_tcp_acceptor),
    make_pair("accept", op::accept)};

  sys_module = {
    make_pair("args", vals::args),
    make_pair("get-dir", op::get_dir),
    make_pair("set-dir", op::set_dir),
    make_pair("mk-dir", op::mk_dir),
    make_pair("remove", op::fs_remove),
    make_pair("remove-all", op::fs_remove_all)};
}

Object *read(Istream *istm) {
  // blank scope
  interp::LexicalScope scope;
  Symbol *cores = language_module->intern("core");
  Module *corem = hydra_cast<Module>(cores->eval(r, scope));
  core_module = corem;

  Symbol* s = corem->intern("read");
  Object *v = s->eval(r, scope);
  Operator *op = hydra_cast<Operator>(v);
  return op->call({istm}, r, scope);
}
