#include "operations/system.hpp"
#include "types.hpp"
#include "utils.hpp"

namespace fs = boost::filesystem;    

using namespace interp;
using namespace expr;

using std::list;


Object* op_get_cwd(list<Object*> arg_list, LocalRuntime& r, LexicalScope &s) {
  return new HString(fs::current_path().string());
}

Object* op_set_cwd(list<Object*> arg_list, LocalRuntime& r, LexicalScope &s) {
  HString* new_wd_str = get_inbuilt<HString*>(arg_list.front());

  fs::path new_wd = fs::path(new_wd_str->value);
  fs::current_path(fs::system_complete( new_wd));
  return nil::get();
}

Object* op_mk_dir(list<Object*> arg_list, LocalRuntime& r, LexicalScope &s) {
  HString* new_dir_str = get_inbuilt<HString*>(arg_list.front());

  boost::filesystem::path dir(new_dir_str->value);
  if (boost::filesystem::create_directory(dir)) {
    return t::get();
  } else {
    return nil::get();
  }
}

Object* fs_op_remove(list<Object*> arg_list, LocalRuntime& r, LexicalScope &s) {
  HString* file_name = get_inbuilt<HString*>(arg_list.front());
  boost::filesystem::path file(file_name->value);
  remove(file);
  return nil::get();
}

Object* fs_op_remove_all(list<Object*> arg_list, LocalRuntime& r, LexicalScope &s) {
  HString* dir_name = get_inbuilt<HString*>(arg_list.front());
  boost::filesystem::path dir(dir_name->value);
  remove_all(dir);
  return nil::get();
}

expr::Operator *op::get_dir;
expr::Operator *op::set_dir;
expr::Operator *op::mk_dir;
expr::Operator *op::fs_remove;
expr::Operator *op::fs_remove_all;

void op::initialize_system() {
  op::get_dir = new InbuiltOperator("get-dir",
                                    "Gets the canonical path of the current\n"
                                    "working directory",
                                    op_get_cwd,
                                    type::Fn::with_args({}),
                                    true);

  op::set_dir = new InbuiltOperator("set-dir",
                                    "Sets the current working directory to the\n"
                                    "provided string",
                                    op_set_cwd,
                                    type::Fn::with_args({type::string_type}),
                                    true);

  op::mk_dir = new InbuiltOperator("mk-dir",
                                    "Creates a new directory given a path",
                                    op_mk_dir,
                                    type::Fn::with_args({type::string_type}),
                                    true);

  op::fs_remove = new InbuiltOperator("remove",
                                   "Deltes a file/directory",
                                    fs_op_remove,
                                    type::Fn::with_args({type::string_type}),
                                    true);

  op::fs_remove_all = new InbuiltOperator("remove-all",
                                    "Recursively deletes a file/directory",
                                    fs_op_remove_all,
                                    type::Fn::with_args({type::string_type}),
                                    true);
}
