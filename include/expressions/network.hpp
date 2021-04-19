#ifndef __HYDRA_EXPRESSIONS_SOCKET_HPP
#define __HYDRA_EXPRESSIONS_SOCKET_HPP

#include <string>

#include "expressions/data/char.hpp"
#include "expressions/object.hpp"
#include "expressions/parent.hpp"
#include "interpreter.hpp"

#include <boost/array.hpp>
#include <boost/asio.hpp>

namespace expr {

struct Socket : public Object {
  Socket(boost::asio::ip::tcp::socket *);
  ~Socket();

  expr::Char *next();
  expr::Char *peek();
  void write(expr::Object *obj, interp::LocalRuntime &r,
                      interp::LexicalScope &s);

  std::string to_string(interp::LocalRuntime &r, interp::LexicalScope &s);

  static Parent *parent;

  boost::asio::ip::tcp::socket *isocket;
  boost::array<char, 128> buff;
  int index;
  int size;
};

struct Acceptor : public Object {
  Acceptor(boost::asio::ip::tcp::acceptor *);
  ~Acceptor();

  expr::Object *accept(Socket *sock);

  std::string to_string(interp::LocalRuntime &r, interp::LexicalScope &s);

  static Parent *parent;

  boost::asio::ip::tcp::acceptor *iacceptor;
};

} // namespace expr

#endif
