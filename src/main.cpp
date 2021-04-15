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

using namespace std;
using namespace interp;
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

// net
// concurrent / parallel


Runtime g;
LocalRuntime r(g);

Object *read(Istream *istm);
void make_modules();

int main(int argc, char **argv) {
  Object::collector = new STWCollector(g);

  // SETUP PARENTS
  // As hydra is prototype-based, objects of each "type" (integer, string, ...)
  // must notionally inherit from some parent object - the "string-parent" or
  // "integer parent", etc. We create them here.

  // Common-bechaviour and default-behaviour are two special cases:
  // common-behaviour acts a parent to almost all objects
  Object::common_behaviour = new Parent("Common Behaviour");
  Object::default_behaviour = new Parent("Default Behaviour");

  // Here are the rest of the "normal" types
  Integer::parent = new Parent("integer-parent");
  Float::parent = new Parent("float-parent");
  Number::parent = new Parent("number-parent");

  HString::parent = new Parent("string-parent");
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
  IOstream::parent->slots[get_keyword("istream")] = Istream::parent;
  IOstream::parent->slots[get_keyword("ostream")] = Ostream::parent;
  Integer::parent->slots[get_keyword("parent")] = Number::parent;
  Float::parent->slots[get_keyword("parent")] = Number::parent;

  IOstream::parent->parents.insert(get_keyword("istream"));
  IOstream::parent->parents.insert(get_keyword("ostream"));
  Integer::parent->parents.insert(get_keyword("parent"));
  Float::parent->parents.insert(get_keyword("parent"));

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
  op::initialize_logic();
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
  op::initialize_io();
  op::initialize_foreign();
  op::initialize_concurrency();

  // we place read last because it uses other operations
  op::initialize_read();

  make_modules();

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
    LexicalScope s;
    try {
      cout << r.active_module->name << "> ";
      ast = read(stm);
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
              make_pair("dev", new Module("dev"))};

  dev = {make_pair("doc", op::describe),
    make_pair("macro-expand", op::macexpand),
    make_pair("time", op::time)};

  op::bin_equal->add(op::obj_eq);
  op::bin_equal->add(op::tuple_eq);
  op::bin_equal->add(op::cons_eq);
  op::bin_equal->add(op::vec_eq);
  op::bin_equal->add(op::str_eq);
  op::bin_equal->add(op::type_eq);
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
    make_pair("set", gn_set),
    make_pair("set-parent", op::obj_pset),
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
    make_pair("binary=", op::bin_equal),
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
    make_pair("Number", type::number_type),
    make_pair("Integer", type::integer_type),
    make_pair("Float", type::float_type),
    make_pair("Nil", type::nil_type),
    make_pair("String", type::string_type),
    make_pair("Char", type::character_type),
    make_pair("Tuple", new type::Tuple),
    make_pair("Vector", new type::Vector),
    make_pair("Type", new type::MetaType),
    make_pair("Module", type::module_type),
    make_pair("Symbol", type::symbol_type),
    make_pair("List", new type::List),
    make_pair("IOStream", type::iostream_type),
    make_pair("IStream", type::istream_type),
    make_pair("OStream", type::ostream_type),
    make_pair("Cons", new type::Cons),
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
