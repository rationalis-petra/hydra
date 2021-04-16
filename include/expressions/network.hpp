#ifndef __HYDRA_EXPRESSIONS_SOCKET_HPP
#define __HYDRA_EXPRESSIONS_SOCKET_HPP

#include <string>

#include "interpreter.hpp"
#include "expressions/data/char.hpp"
#include "expressions/object.hpp"
#include "expressions/parent.hpp"

#include <boost/asio.hpp>
#include <boost/array.hpp>

namespace expr {

struct Socket : public Object {
  Socket(boost::asio::ip::tcp::socket*);
  void mark_node();

  expr::Char* next();
  expr::Char* peek();

  std::string to_string(interp::LocalRuntime &r, interp::LexicalScope& s);

  static Object* parent;

  boost::asio::ip::tcp::socket* isocket;
  boost::array<char, 128> buff;
  int index;
  int size;
};

} 

#endif
