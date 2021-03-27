#include <string>

#include "expressions.hpp"
#include "types.hpp"

using std::string;

//using namespace type;
using namespace type;

Type *type::integer_type;
Type *type::nil_type;
Type *type::character_type;
Type *type::module_type;
Type *type::string_type;
Type *type::istream_type;
Type *type::ostream_type;
Type *type::iostream_type;
Type *type::mirror_type;
Type *type::meta_type;

void type::initialize_types() {
  type::nil_type = new Is(expr::nil::get());

  type::integer_type = new Derives(expr::Integer::parent);
  type::character_type = new Derives(expr::Char::parent);
  //type::module_type = new Derives(expr::Module::parent);
  type::string_type = new Derives(expr::HString::parent);
  // type::istream_type = new Derives(expr::IStream::parent);
  // type::ostream_type = new Derives(expr::OStream::parent);
  //type::iostream_type = new Derives();
  //type::meta_type = new Derives(Type::parent);
  //type::mirror_type = new Derives(expr::Mirror::parent);
}
