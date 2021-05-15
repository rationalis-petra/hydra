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
#include "init.hpp"

using namespace std;

using namespace expr;

extern string lang;

Module *expr::language_module = nullptr;
Module *expr::keyword_module = nullptr;
Module *expr::core_module = nullptr;
GenericFn *expr::equal_operator = nullptr;
GenericFn *expr::gn_to_string = nullptr;

vector<pair<string, Object *>> inbuilts;

vector<pair<string, Object *>> dev_setup;
vector<pair<string, Object *>> core_setup;
vector<pair<string, Object *>> foreign_setup;
vector<pair<string, Object *>> io_setup;
vector<pair<string, Object *>> concurrent_setup;
vector<pair<string, Object *>> network_setup;
vector<pair<string, Object *>> sys_setup;

// net
// concurrent / parallel

interp::Runtime g;
interp::LocalRuntime r(g);

Object *read(Istream *istm);
void make_modules();

int main(int argc, char **argv) {
  Object::collector = new interp::STWCollector(g);

  // We call many functions defined in init.hpp,
  // so the broad strokes of initialization are
  // init();

  // ASSIGN values to Object::parent, Float::parent, etc.
  init_parents();

  // SETUP MODULES
  // We need to specially setup the keyword module because 
  // we don't have the "parent" symbol yet!!
  // so we need to do some special out-of sequence construction to
  // avoid "parent" attempting to intern itself in an endless loop...
  {
    ModuleImpl *key = ModuleImpl::no_parent("keyword");
    keyword_module = key;
    // Symbols are descended from the Symbol-Parent object, which lives
    // in the slot with symbol 'parent'. Hence, we first need to create
    // the parent symbol, then assign it's slot with symbol parent (i.e. itself)
    // to the Symbol-Parent object.
    Symbol *parent_sym = Symbol::symbol_no_parent("parent");
    keyword_module->insert(parent_sym);
    parent_sym->slots[parent_sym] = Symbol::parent;
    parent_sym->parents.insert(parent_sym);


    ModuleImpl *root = ModuleImpl::no_parent("");
    g.root = root;
    Symbol *sym = g.root->intern("keyword");
    sym->value = keyword_module;

    // now, we can set both the root and keyword modules to have their
    // parent be the Module-Parent object
    root->set_parent();
    key->set_parent();
  }

  // we also setup some parent relations for types
  Object::default_behaviour->slots[get_keyword("parent")] = Object::common_behaviour;


  language_module = new ModuleImpl("hydra");
  Symbol* sym = g.root->intern("hydra");
  sym->value = language_module;
  r.active_module = language_module;


  // SETUP type hierarchy
  CType::modifier_parent->set_parent("parent", CType::parent);

  IOstream::parent->set_parent("istream", Istream::parent);
  IOstream::parent->set_parent("ostream", Ostream::parent);
  Socket::parent->set_parent("parent", IOstream::parent);

  Number::parent->set_parent("parent", Object::common_behaviour);
  Complex::parent->set_parent("parent", Number::parent);
  Real::parent->set_parent("parent", Number::parent);
  Integer::parent->set_parent("parent", Real::parent);
  Float::parent->set_parent("parent", Real::parent);


  // SETUP operators
  init_operators();


  // initialize global values so they can be palced in modules
  {
    std::list<Object*> args;
    for (int i = 1; i < argc; i++) {
      args.push_back(new HString(argv[i]));
    }
    vals::args = list_to_cons(args);
  }

  make_modules();

  Istream *hydra_cin = new Istream();
  hydra_cin->stream = &cin;
  sym =
      get_inbuilt<Module*>(language_module->intern("io")->value)->intern("+cin+");
  sym->value = hydra_cin;
  Ostream* hydra_cout = new Ostream();
  hydra_cout->stream = &cout;
  sym =
      get_inbuilt<Module*>(language_module->intern("io")->value)->intern("+cout+");
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
      cout << r.active_module->get_name() << "> ";
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
              make_pair("core", new ModuleImpl("core")),
              make_pair("io", new ModuleImpl("io")),
              make_pair("foreign", new ModuleImpl("foreign")),
              make_pair("concurrent", new ModuleImpl("concurrent")),
              make_pair("system", new ModuleImpl("system")),
              make_pair("dev", new ModuleImpl("dev")),
              make_pair("network", new ModuleImpl("network"))};

  dev_setup = {make_pair("doc", op::describe),
    make_pair("macro-expand", op::macexpand),
    make_pair("time", op::time)};

  op::bin_equal->add(op::type_eq);
  equal_operator = op::equal;

  gn_to_string = new GenericFn;
  gn_to_string->is_fn = true;
  gn_to_string->type->arg_list.push_back(new type::Any);
  gn_to_string->type->return_type = type::string_type;
  gn_to_string->add(op::to_str);

  core_setup = {
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
    make_pair("append", op::cat),
    make_pair("len", op::len),
    // constructors
    make_pair("cmplx", op::mk_cmplx),
    make_pair("tuple", op::mk_tuple),
    make_pair("union", op::mk_union),
    make_pair("vector", op::mk_vec),
    make_pair("cons", op::mk_cons),
    make_pair("list", op::mk_list),

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
    make_pair("set-parent", op::set_slot_parent),
    make_pair("remove-slot", op::delete_slot),

    make_pair("get-meta", op::get_meta),
    make_pair("get-metaslot", op::get_meta_val),
    make_pair("set-metaslot", op::set_meta_val),
    make_pair("remove-metaslot", op::delete_meta),

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

    make_pair("bound?", op::boundp),
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

    make_pair("class", op::mk_typeclass),
    make_pair("type?", op::typep),
    //make_pair("type", op::mk_type),
    make_pair("subtype?", op::subtype)
  };

  Symbol *typesym = get_keyword("type");
  Object* cshortobj = new Object();
  CBasicType* shortctype = new CBasicType(tint);
  shortctype->size = size_short;
  cshortobj->set_invoker(op::mk_short);
  cshortobj->parents.insert(typesym);
  cshortobj->slots[typesym] = shortctype;

  Object* clongobj = new Object();
  CBasicType* longctype = new CBasicType(tint);
  longctype->size = size_long;
  clongobj->set_invoker(op::mk_long);
  clongobj->slots[typesym] = longctype;
  cshortobj->parents.insert(typesym);

  foreign_setup = {
      // ffi
      make_pair("load-c-library", op::foreign_lib),
      make_pair("get-c-symbol", op::foreign_sym),
      make_pair("internalize", op::internalize),

      // simple types
      make_pair("int", new CBasicType(tint)),
      make_pair("char", new CBasicType(tchar)),
      make_pair("float", new CBasicType(tfloat)),
      make_pair("double", new CBasicType(tdouble)),
      make_pair("void", new CVoidType()),

      // type modifiers
      make_pair("short", cshortobj),
      make_pair("long", clongobj),
      make_pair("signed", op::mk_signed),
      make_pair("unsigned", op::mk_unsigned),

      // composite types
      make_pair("fn", op::mk_cfntype),
      make_pair("ptr", op::mk_ptrtype),

      // proxy
      make_pair("proxy", op::mk_proxy),
      make_pair("enum", op::mk_enumtype),

      // the C metatype
      make_pair("CType", type::c_type_type)
      // make_pair("CProxy", type::c_proxy_type),
  };

  io_setup = {// io
        make_pair("print", op::print),
        make_pair("open-file", op::open_file),
        make_pair("close", op::close)};

  concurrent_setup = {
    make_pair("thread", op::thread),
    make_pair("mutex", op::mk_mutex),
    make_pair("lock", op::lock_mutex),
    make_pair("unlock", op::unlock_mutex)};

  network_setup = {
    make_pair("tcp-socket", op::mk_tcp_socket),
    make_pair("tcp-acceptor", op::mk_tcp_acceptor),
    make_pair("accept", op::accept)};

  sys_setup = {
    make_pair("args", vals::args),
    make_pair("get-dir", op::get_dir),
    make_pair("set-dir", op::set_dir),
    make_pair("mk-dir", op::mk_dir),
    make_pair("remove", op::fs_remove),
    make_pair("remove-all", op::fs_remove_all)};


  // arithmetic

  for (auto p : inbuilts) {
    Symbol *sym = language_module->intern(p.first);
    sym->value = p.second;
  }

  vector<pair<string, vector<pair<string, Object *>>>> moddefs = {
      make_pair("core", core_setup),
      make_pair("io", io_setup),
      make_pair("foreign", foreign_setup),
      make_pair("dev", dev_setup),
      make_pair("concurrent", concurrent_setup),
      make_pair("network", network_setup),
      make_pair("system", sys_setup)};

  for (auto m : moddefs) {
    Module* mod = dynamic_cast<Module*>(
        dynamic_cast<Symbol*>(language_module->get(m.first))->value);
    for (auto p : m.second) {
      Symbol *sym = mod->intern(p.first);
      mod->export_sym(sym->name);
      sym->value = p.second;
    }
  }

}

Object *read(Istream *istm) {
  // blank scope
  interp::LexicalScope scope;
  Symbol *cores = language_module->intern("core");
  Module *corem = get_inbuilt<Module*>(cores->eval(r, scope));
  core_module = corem;

  Symbol* s = corem->intern("read");
  Object *v = s->eval(r, scope);
  Operator *op = get_inbuilt<Operator*>(v);
  return op->call({istm}, r, scope);
}
