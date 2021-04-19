#include <string>

#include "expressions.hpp"
#include "types.hpp"

using std::string;

//using namespace type;
using namespace type;

Type *type::number_type;
Type *type::complex_type;
Type *type::real_type;
Type *type::integer_type;
Type *type::float_type;

Type *type::nil_type;
Type *type::character_type;
Type *type::module_type;
Type *type::string_type;
Type *type::mirror_type;
Type *type::meta_type;
Type *type::symbol_type;
Type *type::mutex_type;

Type *type::istream_type;
Type *type::ostream_type;
Type *type::iostream_type;
Type *type::socket_type;
Type *type::acceptor_type;

Type* type::common_type;
Type* type::default_type;

void type::initialize_types() {
  type::nil_type = new Is(expr::nil::get());

  type::number_type = new Derives(expr::Number::parent);
  type::complex_type = new Derives(expr::Complex::parent);
  type::real_type = new Derives(expr::Real::parent);
  type::integer_type = new Derives(expr::Integer::parent);
  type::float_type = new Derives(expr::Float::parent);

  type::mutex_type = new Derives(expr::Mutex::parent);
  type::character_type = new Derives(expr::Char::parent);
  type::module_type = new Derives(expr::Module::parent);
  type::string_type = new Derives(expr::HString::parent);
  // type::meta_type = new Derives(Type::parent);
  type::mirror_type = new Derives(expr::Mirror::parent);
  type::symbol_type = new Derives(expr::Symbol::parent);

  type::istream_type = new Derives(expr::Istream::parent);
  type::ostream_type = new Derives(expr::Ostream::parent);
  type::iostream_type = new Derives(expr::IOstream::parent);
  type::socket_type = new Derives(expr::Socket::parent);
  type::acceptor_type = new Derives(expr::Acceptor::parent);

  type::common_type = new Derives(expr::Object::common_behaviour);
  type::default_type = new Derives(expr::Object::default_behaviour);
}
