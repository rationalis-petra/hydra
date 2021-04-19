#include <string>

#include <boost/asio.hpp>

#include "expressions.hpp"
#include "utils.hpp"

using namespace expr;

Parent *Socket::parent;

Socket::Socket(boost::asio::ip::tcp::socket *socket)
  : isocket(socket), index(0), size(0) {
  slots[get_keyword("parent")] = parent;
  parents.insert(get_keyword("parent"));
}

Socket::~Socket() {
  delete isocket;
}

std::string Socket::to_string(interp::LocalRuntime &r,
                              interp::LexicalScope &s) {
  return "<tcp socket>";
}

Char* Socket::next() {
  boost::system::error_code error;
  if (index == size) {
    size = isocket->read_some(boost::asio::buffer(buff), error);
    index = 0;
  } else {
    index++;
  }
  return new Char(buff[index]);
}

Char* Socket::peek() {
  return new Char(buff[index]);
}

void Socket::write(expr::Object *obj, interp::LocalRuntime &r,
                      interp::LexicalScope &s) {
  std::string msg = hydra_to_string(obj, r, s);
  boost::system::error_code ignored_error;
  boost::asio::write(*isocket, boost::asio::buffer(msg), ignored_error);
}
                                              














Parent* Acceptor::parent;

Acceptor::Acceptor(boost::asio::ip::tcp::acceptor* acceptor)
  : iacceptor(acceptor) {
  slots[get_keyword("parent")] = parent;
  parents.insert(get_keyword("parent"));
}

Acceptor::~Acceptor() {
  delete iacceptor;
}

expr::Object *Acceptor::accept(Socket *sock) {
  iacceptor->accept(*(sock->isocket));
  return sock;
}

std::string Acceptor::to_string(interp::LocalRuntime &r, interp::LexicalScope &s) {
  return "<tcp acceptor>";
}

